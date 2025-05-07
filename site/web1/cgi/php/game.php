#!/usr/bin/php
<?php
echo "Content-Type: text/html\r\n";
echo "\r\n";

$reqBody = file_get_contents("php://stdin");
parse_str($reqBody, $_POST);
echo '<html><h1 style="text-align: center; font-family: Arial, sans-serif; font-size: 25px; color: black;">';

if (isset($_POST['content'])) {
    $userChoice = strtolower($_POST['content']);
    
    if (!in_array($userChoice, ['rock', 'paper', 'scissors'])) {
        echo "Invalid choice. Please choose rock, paper, or scissors.";
        echo '<br><br><a href="/">Return to Home</h1></html>';
        exit;
    }
    $choices = ['rock', 'paper', 'scissors'];
    $serverChoice = $choices[array_rand($choices)];

    if ($userChoice == $serverChoice) {
        echo "Server chose: $serverChoice <br><br>";
        echo "It's a tie!";
    } elseif (
        ($userChoice == 'rock' && $serverChoice == 'scissors') ||
        ($userChoice == 'scissors' && $serverChoice == 'paper') ||
        ($userChoice == 'paper' && $serverChoice == 'rock')
    ) {
        echo "Server chose: $serverChoice <br><br>";
        echo '<span style="color: red;">You win!</span>';
        
    } else {
        echo "Server chose: $serverChoice <br><br>";
        echo '<span style="color: red;">You lose!</span>';
    }
} else {
    echo "No input";
}
echo '<br><br><a href="/">Return to Home</h1></html>';
?>
