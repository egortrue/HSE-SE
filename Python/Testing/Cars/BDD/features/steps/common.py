import selenium.common.exceptions
from behave import *
from selenium import webdriver

import time

WEBDRIVER_PATH = "D:/Program Files/ChromeDriver/96/chromedriver.exe"
WEBSITE_URL = "file:///D:/Code/HSE-SE/Python/Testing/Cars/html/CarConfig.htm"


@given("car purchase website")
def step_impl(context):
    chrome_options = webdriver.ChromeOptions()
    chrome_options.add_argument("--log-level=3")
    chrome_options.add_argument("--mute-audio")
    chrome_options.add_argument("--no-sandbox")
    chrome_options.add_experimental_option('excludeSwitches', ['enable-logging'])
    driver = webdriver.Chrome(WEBDRIVER_PATH, chrome_options=chrome_options)
    driver.get(WEBSITE_URL)
    time.sleep(1)

    context.driver = driver


@when("vehicles dialog is opened")
def step_impl(context):
    driver = context.driver
    driver.find_element_by_id("mOptions_menu").click()
    driver.find_element_by_id("miVehicles_menu").click()
    time.sleep(1)


@step("vehicles dialog is closed")
def step_impl(context):
    driver = context.driver
    driver.find_element_by_id("OkButton1").click()
    time.sleep(1)


@step("car is available")
def step_impl(context):
    driver = context.driver
    row = 2
    while True:
        name = driver.find_element_by_xpath(f"/html/body/center/div/div[2]/div/div[1]/table/tbody/tr[{row}]/td[1]").text
        price = driver.find_element_by_xpath(f"/html/body/center/div/div[2]/div/div[1]/table/tbody/tr[{row}]/td[2]").text.split('.')[0]
        if name == context.name and price == context.price:
            break
        row += 1


@step("car is not available")
def step_impl(context):
    driver = context.driver

    row = 2
    try:
        while True:
            name = driver.find_element_by_xpath(f"/html/body/center/div/div[2]/div/div[1]/table/tbody/tr[{row}]/td[1]").text
            price = driver.find_element_by_xpath(f"/html/body/center/div/div[2]/div/div[1]/table/tbody/tr[{row}]/td[2]").text.split('.')[0]
            if name == context.name and price == context.price:
                break
            row += 1
    except selenium.common.exceptions.NoSuchElementException:
        return
    raise Exception


@then("input is invalid")
def step_impl(context):
    driver = context.driver
    attributes = driver.find_element_by_xpath("/html/body/div[13]").get_attribute("style")

    attrs_dict = dict()
    for keyval in attributes.split(';'):
        t = keyval.split(':')
        if len(t) > 1:
            key = t[0].strip()
            val = t[1].strip()
            attrs_dict[key] = val

    assert attrs_dict['display'] != 'none'