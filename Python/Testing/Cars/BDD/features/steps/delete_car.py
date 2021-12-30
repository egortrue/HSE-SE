from behave import *
from selenium.webdriver.common.alert import Alert

import time


@step("car is deleted via edit column")
def step_impl(context):
    driver = context.driver
    context.name = driver.find_element_by_xpath("/html/body/div[1]/div[2]/div[1]/table/tbody/tr[2]/td[1]").text
    context.price = driver.find_element_by_xpath("/html/body/div[1]/div[2]/div[1]/table/tbody/tr[2]/td[2]").text.split('.')[0]

    driver.find_element_by_xpath("/html/body/div[1]/div[2]/div[1]/table/tbody/tr[2]/td[3]/a[1]").click()
    time.sleep(1)


@step("car is deleted via button")
def step_impl(context):
    driver = context.driver
    context.name = driver.find_element_by_xpath("/html/body/div[1]/div[2]/div[1]/table/tbody/tr[2]/td[1]").text
    context.price = driver.find_element_by_xpath("/html/body/div[1]/div[2]/div[1]/table/tbody/tr[2]/td[2]").text.split('.')[0]

    driver.find_element_by_id("DeleteButton1").click()
    time.sleep(1)


@step("deletion is accepted")
def step_impl(context):
    Alert(context.driver).accept()
    time.sleep(1)

