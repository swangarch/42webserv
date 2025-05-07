#!/usr/bin/php
<?php

echo "Content-Type: text/html\r\n";
echo "Status: 200 OK\r\n";
echo "\r\n";

$reqBody = file_get_contents("php://stdin");

echo '<html><h1 style="text-align: center; font-family: Arial, sans-serif; font-size: 25px; color: black;">';

parse_str($reqBody, $_POST);

if (isset($_POST['content'])) {
    $content = $_POST['content'];
    echo "Content: " . htmlspecialchars($content);
}
else {
    echo "No content";
}

echo '<br><br><a href="/">Return to Home</h1></html>';
?>
