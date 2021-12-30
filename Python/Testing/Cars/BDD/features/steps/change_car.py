from behave import *
from selenium import webdriver
from selenium.webdriver.common.keys import Keys

import time


@step("edit dialog is opened")
def step_impl(context):
    context.driver.find_element_by_xpath("/html/body/div[1]/div[2]/div[1]/table/tbody/tr[2]/td[3]/a[2]").click()
    time.sleep(1)


@step("edit dialog is closed")
def step_impl(context):
    context.driver.find_element_by_id("OkButton3").click()
    time.sleep(1)


@step("vehicles dialog is pointed")
def step_impl(context):
    context.driver.find_element_by_xpath("/html/body/div[1]/div[2]/div[1]/table/tbody/tr[2]/td[1]").click()
    time.sleep(1)


@step("vehicles dialog is cleared")
def step_impl(context):
    driver = context.driver
    driver.find_element_by_id("VehicleName_input").clear()
    driver.find_element_by_id("VehiclePrice_input").clear()


@step('car is edited to name "{new_name}" and price "{new_price}" via button')
def step_impl(context, new_name, new_price):
    driver = context.driver
    driver.find_element_by_id("VehicleName_input").send_keys(new_name)
    driver.find_element_by_id("VehiclePrice_input").send_keys(new_price)
    driver.find_element_by_id("ChangeButton1").click()
    time.sleep(1)

    context.name = new_name
    context.price = new_price


@step("edit dialog is cleared")
def step_impl(context):
    driver = context.driver
    driver.find_element_by_id("VehicleDialogEditPanelVehicleName_input").clear()
    driver.find_element_by_id("VehicleDialogEditPanelVehiclePrice_input").clear()


@step('car is edited to name "{new_name}" and price "{new_price}" via dialog')
def step_impl(context, new_name, new_price):
    driver = context.driver
    driver.find_element_by_id("VehicleDialogEditPanelVehicleName_input").send_keys(new_name)
    driver.find_element_by_id("VehicleDialogEditPanelVehiclePrice_input").send_keys(new_price)
    time.sleep(1)

    context.name = new_name
    context.price = new_price

