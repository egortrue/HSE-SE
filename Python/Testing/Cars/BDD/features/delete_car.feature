
Feature: Delete car

  Scenario: Delete car via edit column
    Given car purchase website
    When vehicles dialog is opened
     And vehicles dialog is pointed
     And car is deleted via edit column
     And deletion is accepted
    Then vehicles dialog is closed
     And car is not available

  Scenario: Delete car via button
    Given car purchase website
    When vehicles dialog is opened
     And vehicles dialog is pointed
     And car is deleted via button
     And deletion is accepted
    Then vehicles dialog is closed
     And car is not available
