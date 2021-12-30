
Feature: Change car description

  Scenario: Change car description via change button
    Given car purchase website
    When vehicles dialog is opened
     And vehicles dialog is pointed
     And vehicles dialog is cleared
     And car is edited to name "moskvich" and price "50" via button
    Then vehicles dialog is closed
     And car is available

  Scenario: Change car description via edit dialog
    Given car purchase website
    When vehicles dialog is opened
     And edit dialog is opened
     And edit dialog is cleared
     And car is edited to name "moskvich" and price "50" via dialog
    Then edit dialog is closed
     And vehicles dialog is closed
     And car is available
