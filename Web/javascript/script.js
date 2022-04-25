// Page objects
var cells, grid, statusMessage, restartButton;

// Local variables
var gameStopped = true;
var turnCounter = 0;
var currentPlayer = "X";
const winning_combinations = [
    [0, 1, 2],
    [3, 4, 5],
    [6, 7, 8],
    [0, 3, 6],
    [1, 4, 7],
    [2, 5, 8],
    [0, 4, 8],
    [2, 4, 6]
];

document.addEventListener('DOMContentLoaded', () => {
    cells = document.querySelectorAll('.cell');
    statusMessage = document.getElementById('statusMessage');
    grid = document.getElementById('grid');
    grid.addEventListener("click", playerInteraction)
    restartButton = document.getElementById('restartButton');
    restartButton.addEventListener("click", startGame);

    startGame();
});

function startGame() {
    gameStopped = false;
    turnCounter = 0;
    cells.forEach(cell => {
        cell.innerHTML = "";
    });
    updateCurrentPlayer();
    updateStatusMessage();
}

function playerInteraction(event) {
    let target = event.target
    if (!gameStopped) {
        if (target.className == "cell" && target.innerHTML == "") {
            target.innerHTML = currentPlayer;

            if (checkWin()) {
                gameStopped = true;
                updateStatusMessage();
                return;
            }

            turnCounter += 1;
            if (turnCounter == 9)
                gameStopped = true;

            updateCurrentPlayer();
            updateStatusMessage();
        }
    }
}

function checkWin() {
    let cells_text = Array.from(cells).map(x => x.innerHTML);
    for (let win of winning_combinations) {
        const a = cells_text[win[0]];
        const b = cells_text[win[1]];
        const c = cells_text[win[2]];
        if (a === '' || b === '' || c === '')
            continue;
        if (a === b && b === c)
            return true;
    }
    return false;
}

function updateStatusMessage() {
    let message = "Status: "
    if (gameStopped) {
        if (turnCounter == 9)
            message += "UNBELIVABLE, It's a draw!";
        else
            message += "Player " + currentPlayer + " won!";
    }
    else
        message += "Player " + currentPlayer + "'s turn";
    statusMessage.innerHTML = message;
    
}

function updateCurrentPlayer() {
    currentPlayer = turnCounter % 2 == 0 ? "X" : "O";
}
