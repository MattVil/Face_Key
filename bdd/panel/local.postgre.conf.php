<?php
    $host = 'localhost';
    $port = '5432';
    $database = 'lharidonlouis';
    $user = 'lharidonlouis';
    $password = '';
    $connectString = 'host=' . $host . ' port=' . $port . ' dbname=' . $database .
        ' user=' . $user . ' password=' . $password;


    $link = pg_connect($connectString);
    if (!$link)
        {
        die('Error: Could not connect: ' . pg_last_error());
    }
?>