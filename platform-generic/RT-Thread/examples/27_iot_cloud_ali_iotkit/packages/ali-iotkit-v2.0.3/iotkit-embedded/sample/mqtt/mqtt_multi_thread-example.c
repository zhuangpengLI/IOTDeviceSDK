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

#ifdef _PLATFORM_IS_LINUX_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "iot_import.h"
#include "iot_export.h"

#include <pthread.h>
#include <assert.h>

#define PRODUCT_KEY             "yfTuLfBJTiL"
#define DEVICE_NAME             "TestDeviceForDemo"
#define DEVICE_SECRET           "fSCl9Ns5YPnYN8Ocg0VEel1kXFnRlV6c"

/* These are pre-defined topics */
#define TOPIC_UPDATE            "/"PRODUCT_KEY"/"DEVICE_NAME"/update"
#define TOPIC_ERROR             "/"PRODUCT_KEY"/"DEVICE_NAME"/update/error"
#define TOPIC_GET               "/"PRODUCT_KEY"/"DEVICE_NAME"/get"
#define TOPIC_DATA              "/"PRODUCT_KEY"/"DEVICE_NAME"/data"

#define MQTT_MSGLEN             (1024)

char g_product_key[PRODUCT_KEY_LEN + 1];
char g_product_secret[PRODUCT_SECRET_LEN + 1];
char g_device_name[DEVICE_NAME_LEN + 1];
char g_device_secret[DEVICE_SECRET_LEN + 1];

#define EXAMPLE_TRACE(fmt, ...)  \
    do { \
        HAL_Printf("%s|%03d :: ", __func__, __LINE__); \
        HAL_Printf(fmt, ##__VA_ARGS__); \
        HAL_Printf("%s", "\r\n"); \
    } while(0)


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

void* thread_subscribe1(void *pclient)
{
    int   ret = -1;
    int cnt = 400;
    
    while(--cnt) {
        HAL_SleepMs(100);
        ret = IOT_MQTT_Subscribe(pclient, TOPIC_DATA, IOTX_MQTT_QOS1, _demo_message_arrive, NULL);
        if (ret < 0) {
            printf("subscribe error");
            return NULL;
        }
        HAL_SleepMs(20);
        ret = IOT_MQTT_Unsubscribe(pclient, TOPIC_GET);
        if (ret < 0) {
            printf("subscribe error");
            return NULL;
        }
        HAL_SleepMs(100);
    }
    return NULL;
}

void* thread_subscribe2(void *pclient)
{
    int   ret = -1;
    int cnt = 400;
    
    while(--cnt) {
        HAL_SleepMs(300);
        ret = IOT_MQTT_Unsubscribe(pclient, TOPIC_DATA);
        if (ret < 0) {
            printf("subscribe error");
            return NULL;
        }
        HAL_SleepMs(30);
        ret = IOT_MQTT_Subscribe(pclient, TOPIC_GET, IOTX_MQTT_QOS1, _demo_message_arrive, NULL);
        if (ret < 0) {
            printf("subscribe error");
            return NULL;
        }
        HAL_SleepMs(30);
    }
    return NULL;
}

// 多线程subscribe
void CASE2(void * pclient)
{
    int   ret = -1;
    pthread_t  pid1;
    pthread_t  pid2;

    if (pclient == NULL) {
        printf("param error");
        return;
    }

    ret = pthread_create(&pid1, NULL, thread_subscribe1, (void*)pclient);
    if (ret != 0) {
        printf("pthread_create failed!\n");
        return;
    }

    ret = pthread_create(&pid2, NULL, thread_subscribe2, (void*)pclient);
    if (ret != 0) {
        printf("pthread_create failed!\n");
        return;
    }
}



void* thread_publish1(void *pclient)
{
    int cnt = 400;
    int ret = -1;
    char msg_pub[MQTT_MSGLEN] = {0};
    iotx_mqtt_topic_info_t topic_msg;

    strcpy(msg_pub, "11111 message: hello! start!");
    topic_msg.qos = IOTX_MQTT_QOS1;
    topic_msg.retain = 0; topic_msg.dup = 0;
    topic_msg.payload = (void *)msg_pub;
    topic_msg.payload_len = strlen(msg_pub);

    while(--cnt) {
        ret = IOT_MQTT_Publish(pclient, TOPIC_DATA, &topic_msg);
        printf("thread<%d>:ret = %d\n", (int)pthread_self(), ret);
        HAL_SleepMs(300);
    }
    
    return NULL;
}

void* thread_publish2(void *pclient)
{
    int cnt = 600;
    int ret = -1;
    char msg_pub[MQTT_MSGLEN] = {0};
    iotx_mqtt_topic_info_t topic_msg;

    strcpy(msg_pub, "22222 message: hello! start!");
    topic_msg.qos = IOTX_MQTT_QOS1;
    topic_msg.retain = 0; topic_msg.dup = 0;
    topic_msg.payload = (void *)msg_pub;
    topic_msg.payload_len = strlen(msg_pub);

    while(--cnt) {
        ret = IOT_MQTT_Publish(pclient, TOPIC_DATA, &topic_msg);
        printf("thread<%d>:ret = %d\n", (int)pthread_self(), ret);
        HAL_SleepMs(200);
    }
    
    return NULL;
}

// 多线程publish
void CASE1(void * pclient)
{
    int   ret = -1;
    pthread_t  pid1;
    pthread_t  pid2;

    if (pclient == NULL) {
        printf("param error");
        return;
    }

    ret = IOT_MQTT_Subscribe(pclient, TOPIC_DATA, IOTX_MQTT_QOS1, _demo_message_arrive, NULL);
    if (ret < 0) {
        printf("subscribe error");
        return;
    }

    ret = pthread_create(&pid1, NULL, thread_publish1, (void*)pclient);
    if (ret != 0) {
        printf("pthread_create failed!\n");
        return;
    }

    ret = pthread_create(&pid2, NULL, thread_publish2, (void*)pclient);
    if (ret != 0) {
        printf("pthread_create failed!\n");
        return;
    }
}


// yield thread
static int yield_exit = 0;

void *thread_yield(void *pclient)
{
    while(yield_exit == 0) {
        IOT_MQTT_Yield(pclient, 200);
        
        HAL_SleepMs(200);
    }

    return NULL;
}


int mqtt_client(void)
{
    int rc = 0;//, msg_len, cnt = 0;
    void *pclient;
    iotx_conn_info_pt pconn_info;
    iotx_mqtt_param_t mqtt_params;
    char *msg_buf = NULL, *msg_readbuf = NULL;
    pthread_t  pid1;
        
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

    /**< get device info*/
    HAL_GetProductKey(g_product_key);
    HAL_GetDeviceName(g_device_name);
    HAL_GetDeviceSecret(g_device_secret);
    /**< end*/
    /* Device AUTH */
    if (0 != IOT_SetupConnInfo(g_product_key, g_device_name, g_device_secret, (void **)&pconn_info)) {
        EXAMPLE_TRACE("AUTH request failed!");
        rc = -1;
        goto do_exit;
    }

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
    
    EXAMPLE_TRACE("TEST CASE");
    pthread_create(&pid1, NULL, thread_yield, (void*)pclient);

    // mutli thread publish
    CASE1(pclient);

    // mutli thread subscribe
    CASE2(pclient);
    
    HAL_SleepMs(100000);
    IOT_MQTT_Unsubscribe(pclient, TOPIC_DATA);
    IOT_MQTT_Unsubscribe(pclient, TOPIC_GET);

    HAL_SleepMs(200);
    yield_exit = 1;
    HAL_SleepMs(200);
    

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


int main(int argc, char **argv)
{
    IOT_OpenLog("mqtt");
    IOT_SetLogLevel(IOT_LOG_DEBUG);
    /**< set device info*/
    HAL_SetProductKey(PRODUCT_KEY);
    HAL_SetDeviceName(DEVICE_NAME);
    HAL_SetDeviceSecret(DEVICE_SECRET);
    /**< end*/
    mqtt_client();
    IOT_DumpMemoryStats(IOT_LOG_DEBUG);
    IOT_CloseLog();

    EXAMPLE_TRACE("out of sample!");

    return 0;
}

#else

int main(int argc, char **argv)
{
    /*EXAMPLE_TRACE("not support, pleasae support multi-thread first!!!");
    
    EXAMPLE_TRACE("Linux is support, please build in ubuntu.x86");*/

    return 0;
}
#endif

