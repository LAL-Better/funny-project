#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @Time     : 2020/8/24 5:15 下午
# @Author   : Colin.Liu
# @Email    : colin50631@gmail.com
# @File     : demo.py
# @Software : PyCharm


import arrow
import requests
import random

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
                result['dayweather'] = record['dayweather']
                result['nightweather'] = record['nightweather']
                result['daytemp'] = record['daytemp']
                result['nighttemp'] = record['nighttemp']
                break
        return result

if __name__ == "__main__":
    ones = OneStatementOfEnglish()
    
    weather = ones.get_weather()
#    print("天气预测：", weather)

    res = ['youdao', 'shanbay', 'jinshan']
    random_int = random.randint(0,2)
    soup = ones.switchToCase(res[random_int])()
    desp = "#### {date}\n\n白天{dayweather}，夜晚{nightweather}，温度{nighttemp}℃ ~ {daytemp}℃。\n\n" \
           "*{content}*\n\n{note}\n\n![]({picture})" \
        .format(date=weather['date'],
                dayweather=weather['dayweather'],
                nightweather=weather['nightweather'],
                nighttemp=weather['nighttemp'],
                daytemp=weather['daytemp'],
                content=soup['content'],
                note=soup['translation'],
                picture=soup['image'])
    print(desp)
