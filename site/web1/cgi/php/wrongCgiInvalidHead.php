#!/usr/bin/php
<?php

echo "Content-Type: text/html\r\n";
echo "Status: 200 NOT \r\n";
echo "Set-Cookie: user=abc; Path=/; Max-Age=12312312\r\n";
echo "\r\n";


echo '<html><h1 style="text-align: center; font-family: Arial, sans-serif; font-size: 25px; color: black;">';

echo "Php cgi hello world.";

echo '<br><br><a href="/">Return to Home</h1></html>';
?>