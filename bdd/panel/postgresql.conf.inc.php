<?php
    $host = '10.40.128.23';
    $port = '5432';
    $database = 'db2017l3i_mvilain';
    $user = 'y2017l3i_mvilain';
    $password = 'A123456#';
    $connectString = 'host=' . $host . ' port=' . $port . ' dbname=' . $database .
        ' user=' . $user . ' password=' . $password;


    $link = pg_connect($connectString);
    if (!$link)
        {
        die('Error: Could not connect: ' . pg_last_error());
    }
?>trftgy
