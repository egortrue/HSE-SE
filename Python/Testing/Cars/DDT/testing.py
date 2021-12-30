import pytest
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import Select

import time


WEBDRIVER_PATH = "D:/Program Files/ChromeDriver/96/chromedriver.exe"
WEBSITE_URL = "file:///D:/Code/HSE-SE/Python/Testing/Cars/html/CarConfig.htm"

chrome_options = webdriver.ChromeOptions()
chrome_options.add_argument("--log-level=3")
chrome_options.add_argument("--mute-audio")
chrome_options.add_argument("--no-sandbox")
chrome_options.add_experimental_option('excludeSwitches', ['enable-logging'])

class Tests:

    def choose_vehicle(self, id):
        self.driver.find_element(By.ID, "TabbedPaneVehicle").click()
        time.sleep(0.1)

        self.driver.find_element(By.XPATH, f"/html/body/center/div/div[2]/div/div[1]/table/tbody/tr[{id}]/td[1]").click()

    def choose_special(self, text):
        self.driver.find_element(By.ID, "TabbedPaneSpecialModel").click()
        time.sleep(0.1)

        combo = Select(self.driver.find_element(By.ID, "SpecialsCombo"))
        time.sleep(0.1)
        combo.select_by_visible_text(text)


    def choose_accessory(self, id):
        self.driver.find_element(By.ID, "TabbedPaneAccessory").click()
        time.sleep(0.1)

        self.driver.find_element(By.XPATH, f"/html/body/center/div/div[2]/div/div[3]/form/table/tbody/tr[{id}]/td[1]/input").click()

    def fill_information(self):
        self.driver.find_element(By.ID, "mOrder_menu").click()
        time.sleep(0.1)
        self.driver.find_element(By.ID, "miSendOrder_menu").click()
        time.sleep(0.1)
        self.driver.find_element(By.ID, "SendPurchaseOrderDialogAccordionContactInDesc").click()
        time.sleep(0.1)

        fields = ["surname_input", "firstname_input", "address_input", "zipcode_input",
                  "city_input", "country_input", "number_input", "address_input"] # this is wrong cause one id for two fields
        for i in range(1, 9):
            self.driver.find_element(By.XPATH, f"/html/body/div[11]/div[2]/div/div[5]/form/table/tbody/tr[{i}]/td[3]/input").send_keys("ABC")
        time.sleep(0.1)
        self.driver.find_element(By.ID, "SendOrderButton").click()
        time.sleep(1)

    def check_pdf(self):
        if self.driver.current_url != "file:///D:/Code/HSE-SE/Python/Testing/Cars/html/confirmEN.pdf":
            raise Exception

    @pytest.mark.parametrize("vehicle", (i for i in range(2, 6 + 1)))
    @pytest.mark.parametrize("special", ("- no special model -", "Gomera", "Jazz", "Luxus"))
    @pytest.mark.parametrize("accessory", [i for i in range(2, 9 + 1)])
    def test_start(self, vehicle, special, accessory):
        self.driver = webdriver.Chrome(WEBDRIVER_PATH, options=chrome_options)
        self.driver.get(WEBSITE_URL)

        self.choose_vehicle(vehicle)
        self.choose_special(special)
        self.choose_accessory(accessory)
        self.fill_information()
        self.check_pdf()

        self.driver.quit()
