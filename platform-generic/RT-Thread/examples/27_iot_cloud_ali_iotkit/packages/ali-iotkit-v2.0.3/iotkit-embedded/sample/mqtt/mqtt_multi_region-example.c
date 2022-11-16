/*
 * Copyright (c) 2014-2016 Alibaba Group. All rights reserved.
 * License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>

#include "iot_import.h"
#include "iot_export.h"
#include "exports/iot_export_cmp.h"


/* Singapore Domain */
#define PRODUCT_KEY_SG             "a1eYfJ2QGei"
#define DEVICE_NAME_SG             "Test1"
#define DEVICE_SECRET_SG           "teVDZgu2T3qnDQ2MeUVqGoBZ3qxKPYHL"

/* Japan Domain */
#define PRODUCT_KEY_JP             "a1R9GUtrTPd"
#define DEVICE_NAME_JP             "Test1"
#define DEVICE_SECRET_JP           "cVhDbPYGoSBIpfnFXjJYKXBtVXa19rRY"

/* America Domain */
#define PRODUCT_KEY_US             "a1FGqgmMbcg"
#define DEVICE_NAME_US             "Test1"
#define DEVICE_SECRET_US           "5jGnXkBY7HeHChZPDZzEHcqIeYdnvPkl"

/* Germany Domain */
#define PRODUCT_KEY_GER            "a1jKEbfQ7at"
#define DEVICE_NAME_GER            "Test1"
#define DEVICE_SECRET_GER          "818ExRExkfDGF8p3ElX3DzSFNBzKnE8F"

/* Shanghai Domain */
#if defined(ON_DAILY)
    #define PRODUCT_KEY             "gsYfsxQJgeD"
    #define DEVICE_NAME             "DailyEnvDN"
    #define DEVICE_SECRET           "y1vzFkEgcuXnvkAfm627pwarx4HRNikX"
#else
    #define PRODUCT_KEY             "a1IfbZi3oDt"
    #define DEVICE_NAME             "Test1"
    #define DEVICE_SECRET           "kuzVoswkUIdb9uXm4T8ykIJushFym8RL"
#endif

char __product_key[PRODUCT_KEY_LEN + 1];
char __device_name[DEVICE_NAME_LEN + 1];
char __device_secret[DEVICE_SECRET_LEN + 1];

/* These are pre-defined topics */
#define TOPIC_UPDATE            "/"PRODUCT_KEY"/"DEVICE_NAME"/update"
#define TOPIC_ERROR             "/"PRODUCT_KEY"/"DEVICE_NAME"/update/error"
#define TOPIC_GET               "/"PRODUCT_KEY"/"DEVICE_NAME"/get"
#define TOPIC_DATA              "/"PRODUCT_KEY"/"DEVICE_NAME"/data"

/* These are pre-defined topics format*/
#define TOPIC_UPDATE_FMT            "/%s/%s/update"
#define TOPIC_ERROR_FMT             "/%s/%s/update/error"
#define TOPIC_GET_FMT               "/%s/%s/get"
#define TOPIC_DATA_FMT              "/%s/%s/data"

#define MQTT_MSGLEN             (1024)

#define EXAMPLE_TRACE(fmt, ...)  \
    do { \
        HAL_Printf("%s|%03d :: ", __func__, __LINE__); \
        HAL_Printf(fmt, ##__VA_ARGS__); \
        HAL_Printf("%s", "\r\n"); \
    } while(0)

static int      user_argc;
static char   **user_argv;

void event_handle(void *pcontext, void *pclient, iotx_mqtt_event_msg_pt msg)
{
    uintptr_t packet_id = (uintptr_t)msg->msg;
    iotx_mqtt_topic_info_pt topic_info = (iotx_mqtt_topic_info_pt)msg->msg;

    switch (msg->event_type) {
        case IOTX_MQTT_EVENT_UNDEF:
            EXAMPLE_TRACE("undefined event occur.");
            break;

        case IOTX_MQTT_EVENT_DISCONNECT:
            EXAMPLE_TRACE("MQTT disconnect.");
            break;

        case IOTX_MQTT_EVENT_RECONNECT:
            EXAMPLE_TRACE("MQTT reconnect.");
            break;

        case IOTX_MQTT_EVENT_SUBCRIBE_SUCCESS:
            EXAMPLE_TRACE("subscribe success, packet-id=%u", (unsigned int)packet_id);
            break;

        case IOTX_MQTT_EVENT_SUBCRIBE_TIMEOUT:
            EXAMPLE_TRACE("subscribe wait ack timeout, packet-id=%u", (unsigned int)packet_id);
            break;

        case IOTX_MQTT_EVENT_SUBCRIBE_NACK:
            EXAMPLE_TRACE("subscribe nack, packet-id=%u", (unsigned int)packet_id);
            break;

        case IOTX_MQTT_EVENT_UNSUBCRIBE_SUCCESS:
            EXAMPLE_TRACE("unsubscribe success, packet-id=%u", (unsigned int)packet_id);
            break;

        case IOTX_MQTT_EVENT_UNSUBCRIBE_TIMEOUT:
            EXAMPLE_TRACE("unsubscribe timeout, packet-id=%u", (unsigned int)packet_id);
            break;

        case IOTX_MQTT_EVENT_UNSUBCRIBE_NACK:
            EXAMPLE_TRACE("unsubscribe nack, packet-id=%u", (unsigned int)packet_id);
            break;

        case IOTX_MQTT_EVENT_PUBLISH_SUCCESS:
            EXAMPLE_TRACE("publish success, packet-id=%u", (unsigned int)packet_id);
            break;

        case IOTX_MQTT_EVENT_PUBLISH_TIMEOUT:
            EXAMPLE_TRACE("publish timeout, packet-id=%u", (unsigned int)packet_id);
            break;

        case IOTX_MQTT_EVENT_PUBLISH_NACK:
            EXAMPLE_TRACE("publish nack, packet-id=%u", (unsigned int)packet_id);
            break;

        case IOTX_MQTT_EVENT_PUBLISH_RECVEIVED:
            EXAMPLE_TRACE("topic message arrived but without any related handle: topic=%.*s, topic_msg=%.*s",
                          topic_info->topic_len,
                          topic_info->ptopic,
                          topic_info->payload_len,
                          topic_info->payload);
            break;

        case IOTX_MQTT_EVENT_BUFFER_OVERFLOW:
            EXAMPLE_TRACE("buffer overflow, %s", msg->msg);
            break;

        default:
            EXAMPLE_TRACE("Should NOT arrive here.");
            break;
    }
}

static void _demo_message_arrive(void *pcontext, void *pclient, iotx_mqtt_event_msg_pt msg)
{
    iotx_mqtt_topic_info_pt ptopic_info = (iotx_mqtt_topic_info_pt) msg->msg;

    /* print topic name and topic message */
    EXAMPLE_TRACE("----");
    EXAMPLE_TRACE("packetId: %d", ptopic_info->packet_id);
    EXAMPLE_TRACE("Topic: '%.*s' (Length: %d)",
                  ptopic_info->topic_len,
                  ptopic_info->ptopic,
                  ptopic_info->topic_len);
    EXAMPLE_TRACE("Payload: '%.*s' (Length: %d)",
                  ptopic_info->payload_len,
                  ptopic_info->payload,
                  ptopic_info->payload_len);
    EXAMPLE_TRACE("----");
}


int mqtt_client(void)
{
    int rc = 0, msg_len, cnt = 0;
    void *pclient;
    iotx_conn_info_pt pconn_info;
    iotx_mqtt_param_t mqtt_params;
    iotx_mqtt_topic_info_t topic_msg;
    char msg_pub[128];
    char *msg_buf = NULL, *msg_readbuf = NULL;
    char  topic_update[IOTX_URI_MAX_LEN] = {0};
    char  topic_error[IOTX_URI_MAX_LEN] = {0};
    char  topic_get[IOTX_URI_MAX_LEN] = {0};
    char  topic_data[IOTX_URI_MAX_LEN] = {0};    

    if (NULL == (msg_buf = (char *)HAL_Malloc(MQTT_MSGLEN))) {
        EXAMPLE_TRACE("not enough memory");
        rc = -1;
        goto do_exit;
    }

    if (NULL == (msg_readbuf = (char *)HAL_Malloc(MQTT_MSGLEN))) {
        EXAMPLE_TRACE("not enough memory");
        rc = -1;
        goto do_exit;
    }

    HAL_GetProductKey(__product_key);
    HAL_GetDeviceName(__device_name);
    HAL_GetDeviceSecret(__device_secret);

    /* Device AUTH */
    if (0 != IOT_SetupConnInfo(__product_key, __device_name, __device_secret, (void **)&pconn_info)) {
        EXAMPLE_TRACE("AUTH request failed!");
        rc = -1;
        goto do_exit;
    }

    HAL_Snprintf(topic_update,IOTX_URI_MAX_LEN,TOPIC_UPDATE_FMT,__product_key,__device_name);
    HAL_Snprintf(topic_error,IOTX_URI_MAX_LEN,TOPIC_ERROR_FMT,__product_key,__device_name);
    HAL_Snprintf(topic_get,IOTX_URI_MAX_LEN,TOPIC_GET_FMT,__product_key,__device_name);
    HAL_Snprintf(topic_data,IOTX_URI_MAX_LEN,TOPIC_DATA_FMT,__product_key,__device_name);

    /* Initialize MQTT parameter */
    memset(&mqtt_params, 0x0, sizeof(mqtt_params));

    mqtt_params.port = pconn_info->port;
    mqtt_params.host = pconn_info->host_name;
    mqtt_params.client_id = pconn_info->client_id;
    mqtt_params.username = pconn_info->username;
    mqtt_params.password = pconn_info->password;
    mqtt_params.pub_key = pconn_info->pub_key;

    mqtt_params.request_timeout_ms = 2000;
    mqtt_params.clean_session = 0;
    mqtt_params.keepalive_interval_ms = 60000;
    mqtt_params.pread_buf = msg_readbuf;
    mqtt_params.read_buf_size = MQTT_MSGLEN;
    mqtt_params.pwrite_buf = msg_buf;
    mqtt_params.write_buf_size = MQTT_MSGLEN;

    mqtt_params.handle_event.h_fp = event_handle;
    mqtt_params.handle_event.pcontext = NULL;


    /* Construct a MQTT client with specify parameter */
    pclient = IOT_MQTT_Construct(&mqtt_params);
    if (NULL == pclient) {
        EXAMPLE_TRACE("MQTT construct failed");
        rc = -1;
        goto do_exit;
    }

    /* Initialize topic information */
    memset(&topic_msg, 0x0, sizeof(iotx_mqtt_topic_info_t));
    strcpy(msg_pub, "update: hello! start!");

    topic_msg.qos = IOTX_MQTT_QOS1;
    topic_msg.retain = 0;
    topic_msg.dup = 0;
    topic_msg.payload = (void *)msg_pub;
    topic_msg.payload_len = strlen(msg_pub);

    rc = IOT_MQTT_Publish(pclient, topic_update, &topic_msg);
    if (rc < 0) {
        IOT_MQTT_Destroy(&pclient);
        EXAMPLE_TRACE("error occur when publish");
        rc = -1;
        goto do_exit;
    }

    EXAMPLE_TRACE("\n publish message: \n topic: %s\n payload: \%s\n rc = %d", topic_update, topic_msg.payload, rc);
    
    /* Subscribe the specific topic */
    rc = IOT_MQTT_Subscribe(pclient, topic_data, IOTX_MQTT_QOS1, _demo_message_arrive, NULL);
    if (rc < 0) {
        IOT_MQTT_Destroy(&pclient);
        EXAMPLE_TRACE("IOT_MQTT_Subscribe() failed, rc = %d", rc);
        rc = -1;
        goto do_exit;
    }

    /* Initialize topic information */
    memset(msg_pub, 0x0, 128);
    strcpy(msg_pub, "data: hello! start!");
    memset(&topic_msg, 0x0, sizeof(iotx_mqtt_topic_info_t));
    topic_msg.qos = IOTX_MQTT_QOS1;
    topic_msg.retain = 0;
    topic_msg.dup = 0;
    topic_msg.payload = (void *)msg_pub;
    topic_msg.payload_len = strlen(msg_pub);

    rc = IOT_MQTT_Publish(pclient, topic_data, &topic_msg);
    EXAMPLE_TRACE("\n publish message: \n topic: %s\n payload: \%s\n rc = %d", topic_data, topic_msg.payload, rc);

    IOT_MQTT_Yield(pclient, 200);

    do {
        /* Generate topic message */
        cnt++;
        msg_len = snprintf(msg_pub, sizeof(msg_pub), "{\"attr_name\":\"temperature\", \"attr_value\":\"%d\"}", cnt);
        if (msg_len < 0) {
            EXAMPLE_TRACE("Error occur! Exit program");
            rc = -1;
            break;
        }

        topic_msg.payload = (void *)msg_pub;
        topic_msg.payload_len = msg_len;

        rc = IOT_MQTT_Publish(pclient, topic_data, &topic_msg);
        if (rc < 0) {
            EXAMPLE_TRACE("error occur when publish");
            rc = -1;
            break;
        }
#ifdef MQTT_ID2_CRYPTO
        EXAMPLE_TRACE("packet-id=%u, publish topic msg='0x%02x%02x%02x%02x'...",
                      (uint32_t)rc,
                      msg_pub[0], msg_pub[1], msg_pub[2], msg_pub[3]
                     );
#else
        EXAMPLE_TRACE("packet-id=%u, publish topic msg=%s", (uint32_t)rc, msg_pub);
#endif

        /* handle the MQTT packet received from TCP or SSL connection */
        IOT_MQTT_Yield(pclient, 200);

        /* infinite loop if running with 'loop' argument */
        if (user_argc >= 2 && !strcmp("loop", user_argv[1])) {
            HAL_SleepMs(2000);
            cnt = 0;
        }

    } while (cnt < 1);
        
    IOT_MQTT_Yield(pclient, 200);

    IOT_MQTT_Unsubscribe(pclient, topic_data);

    IOT_MQTT_Yield(pclient, 200);

    IOT_MQTT_Destroy(&pclient);

do_exit:
    if (NULL != msg_buf) {
        HAL_Free(msg_buf);
    }

    if (NULL != msg_readbuf) {
        HAL_Free(msg_readbuf);
    }

    return rc;
}

int ali_mqtt_region_test(int argc, char **argv)
{
    int opt; 
    char region[5];
    int domain_type;

    while ((opt = getopt(argc, argv, "r:")) != -1)
    {
        if ('r' == opt)
        {
            strncpy(region, optarg, (strlen(optarg)>5)?5:strlen(optarg));
        }
        else
        {
            HAL_Printf("usage: mqtt_multi_region-example -r [PARAMTER]\r\n");
            HAL_Printf("\t-r sh \t\tSelect Shanghai demain.\r\n");
            HAL_Printf("\t-r sg \t\tSelect Singapore demain.\r\n");
            HAL_Printf("\t-r jp \t\tSelect Japan demain.\r\n");
            HAL_Printf("\t-r us \t\tSelect America demain.\r\n");
            HAL_Printf("\t-r ger\t\tSelect Germany demain.\r\n");
            return 1;
        }
    }

    if (0 == strncmp(region, "sg", strlen("sg")))
    {
        domain_type = IOTX_CLOUD_DOMAIN_SG;
        HAL_SetProductKey(PRODUCT_KEY_SG);
        HAL_SetDeviceName(DEVICE_NAME_SG);
        HAL_SetDeviceSecret(DEVICE_SECRET_SG);          

        EXAMPLE_TRACE("Singapore Domain selected");
    }
    else if (0 == strncmp(region, "jp", strlen("jp")))
    {
        domain_type = IOTX_CLOUD_DOMAIN_JP;
        HAL_SetProductKey(PRODUCT_KEY_JP);
        HAL_SetDeviceName(DEVICE_NAME_JP);
        HAL_SetDeviceSecret(DEVICE_SECRET_JP);        

        EXAMPLE_TRACE("Japan Domain selected");
    }
    else if (0 == strncmp(region, "us", strlen("us")))
    {
        domain_type = IOTX_CLOUD_DOMAIN_US;
        HAL_SetProductKey(PRODUCT_KEY_US);
        HAL_SetDeviceName(DEVICE_NAME_US);
        HAL_SetDeviceSecret(DEVICE_SECRET_US);          

        EXAMPLE_TRACE("America Domain selected");
    }
    else if (0 == strncmp(region, "ger", strlen("ger")))
    {
        domain_type = IOTX_CLOUD_DOMAIN_GER;
        HAL_SetProductKey(PRODUCT_KEY_GER);
        HAL_SetDeviceName(DEVICE_NAME_GER);
        HAL_SetDeviceSecret(DEVICE_SECRET_GER);          
        
        EXAMPLE_TRACE("Germany Domain selected");
    }
    else 
    {
        domain_type = IOTX_CLOUD_DOMAIN_SH;
        HAL_SetProductKey(PRODUCT_KEY);
        HAL_SetDeviceName(DEVICE_NAME);
        HAL_SetDeviceSecret(DEVICE_SECRET);          

        EXAMPLE_TRACE("Shanghai Domain selected");
    }
    
    IOT_OpenLog("mqtt");
    IOT_SetLogLevel(IOT_LOG_DEBUG);
    IOT_SetupDomain(domain_type);

    mqtt_client();

    IOT_DumpMemoryStats(IOT_LOG_DEBUG);
    IOT_CloseLog();

    EXAMPLE_TRACE("out of sample!");

    return 0;
}

