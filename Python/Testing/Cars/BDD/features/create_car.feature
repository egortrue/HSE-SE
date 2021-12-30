
Feature: Create car

  Scenario: Create car with valid data
    Given car purchase website
    When vehicles dialog is opened
     And car is created with name "moskvich" and price "50"
    Then vehicles dialog is closed
     And car is available

  Scenario: Create car with invalid name
    Given car purchase website
    When vehicles dialog is opened
     And car is created with name " " and price "50"
    Then input is invalid

    Scenario: Create car with invalid price
    Given car purchase website
    When vehicles dialog is opened
     And car is created with name "moskvich" and price " "
    Then input is invalid
