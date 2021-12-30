from behave import *
from selenium import webdriver
from selenium.webdriver.common.keys import Keys

import time


@step('car is created with name "{name}" and price "{price}"')
def step_impl(context, name, price):
    driver = context.driver
    driver.find_element_by_id("VehicleName_input").send_keys(name)
    driver.find_element_by_id("VehiclePrice_input").send_keys(price)
    driver.find_element_by_id("NewButton1").click()
    time.sleep(1)

    context.name = name
    context.price = price

