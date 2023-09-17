#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @Time     : 2023/09/17 5:15 下午
# @Author   : Dengml
# @Email    : dengml1996@163.com
# @File     : 


import arrow
import requests
import random

import smtplib
from email.mime.text import MIMEText
from email.header import Header
import markdown

class OneStatementOfEnglish:
    """
    英语每日一句
    目前支持：
        + 有道
        + 扇贝
        + 金山
    """
    _today = arrow.now().format('YYYY-MM-DD')

    def switchToCase(self, case):
        # print("case: ", case)
        func_name = "case_" + str(case)
        # print("func_name: ", func_name)
        method = getattr(self, func_name, self.switchToCase)
        return method

    def case_youdao(self):
        yd_url = "https://dict.youdao.com/infoline?mode=publish&date=" + self._today + "&update=auto&apiversion=5.0"
        result = {}
        for record in requests.get(yd_url).json()[self._today]:
            if record['type'] == '壹句':
                result['date'] = self._today
                result['content'] = record['title']
                result['translation'] = record['summary']
                result['image'] = record['image'][0]

                break
        return result

    def case_shanbay(self):
        sb_url = "https://apiv3.shanbay.com/weapps/dailyquote/quote/?date=" + self._today
        result = {}
        record = requests.get(sb_url).json()

        result['date'] = self._today
        result['content'] = record['content']
        result['translation'] = record['translation']
        result['image'] = record['share_img_urls'][0]

        return result

    def case_jinshan(self):
        js_url = "http://open.iciba.com/dsapi/"
        result = {}
        record = requests.get(js_url).json()
        result['date'] = self._today
        result['content'] = record['content']
        result['translation'] = record['note']
        result['image'] = record['picture']
        return result

    def get_weather(self):
        weather_url = "https://restapi.amap.com/v3/weather/weatherInfo?city=310107&key=97db0aca85eb51c67f79cb8950926d82&extensions=all"
        result = {}
        for record in requests.get(weather_url).json()['forecasts'][0]['casts']:
            #print(record)
            if record['date'] == self._today:
                result['date'] = self._today
                result['week'] = record['week']
                result['dayweather'] = record['dayweather']
                result['nightweather'] = record['nightweather']
                result['daytemp'] = record['daytemp']
                result['nighttemp'] = record['nighttemp']
                break
        return result

mail_host="smtp.163.com"

def mail_send(usr, passwd, recv, subject, content):
	message = MIMEText(content,_subtype="html",_charset="utf-8")
	message['Subject'] = subject
	message['From'] = usr
	message['To'] = recv
	try:
	    smtpObj = smtplib.SMTP() 
	    smtpObj.connect(mail_host, 25)    # 25 为 SMTP 端口号
	    smtpObj.login(usr,passwd)
	    smtpObj.sendmail(usr, recv.split(','), message.as_string())
	    print ("邮件发送成功")
	except smtplib.SMTPException:
	    print ("Error: 无法发送邮件")
	

if __name__ == "__main__":
    ones = OneStatementOfEnglish()
    
    weather = ones.get_weather()
#    print("天气预测：", weather)

    res = ['youdao', 'shanbay', 'jinshan']
    weekday = ['周一', '周二', '周三', '周四', '周五', '周六', '周日']
    random_int = random.randint(0,2)
    soup = ones.switchToCase(res[random_int])()
    desp = "#### {date} {week}\n\n白天{dayweather}，夜晚{nightweather}，温度{nighttemp}℃ ~ {daytemp}℃。\n\n" \
           "*{content}*\n\n{note}\n\n![]({picture})" \
        .format(date=weather['date'],
                dayweather=weather['dayweather'],
                week=weekday[int(weather['week'])-1],
                nightweather=weather['nightweather'],
                nighttemp=weather['nighttemp'],
                daytemp=weather['daytemp'],
                content=soup['content'],
                note=soup['translation'],
                picture=soup['image'])
    print(desp)
    mail_content = markdown.markdown(desp)
    mail_send('<用户邮箱>', '<用户密码或者授权码>', '<接收者邮箱地址，以逗号隔开>', 'Daily Greatings', mail_content)