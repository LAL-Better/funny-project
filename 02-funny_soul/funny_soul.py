import sqlite3
import random
import smtplib
from email.mime.text import MIMEText
from email.header import Header

#设置服务器所需信息
#163邮箱服务器地址
mail_host = 'smtp.163.com'

mail_user = "dengml1996@163.com"
mail_pass = "dx6971960812"
sender = "dengml1996@163.com"
receivers = ['1253210676@qq.com']

conn = sqlite3.connect('sqlite.db')
print ("数据库打开成功")

cur = conn.cursor()

while True:
	random_int = random.randint(0,1032)
	sql_text = "select title from soul where id=%d;" % (random_int)
	res = cur.execute(sql_text)
	rows = cur.fetchmany()
	if len(rows) == 1:
		print (rows[0][0])
		content = rows[0][0]
		break
cur.close()
conn.close()

message = MIMEText(content,'plain','utf-8')
message['Subject'] = 'Soul message'
message['From'] = sender
message['To'] = receivers

#try:
	#smtpObj = smtplib.SMTP() 
	#smtpObj.connect(mail_host,994)
	#smtpObj = smtplib.SMTP_SSL(mail_host,994)
	#smtpObj.login(mail_user,mail_pass) 
	#smtpObj.sendmail(sender,receivers,message.as_string()) 
	#smtpObj.close()
	#print('success')
#except smtplib.SMTPException as e:
	#print('error',e) #打印错误
