# GPT-MiraiCP
简易 GPT bot 实现，仅限私聊对话

使用的后端为[OpenAISession](https://github.com/Antares0982/OpenAISession)，配置文件格式如下，第一次填写sid全填-1即可：

```json
{
    "permitted": [
        {
            "qqid": 1234567,
            "sid": -1
        }
    ],
    "port": 27000
}
```

