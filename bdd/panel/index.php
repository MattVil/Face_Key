<?php
 include 'local.postgre.conf.php';
  // include 'postgresql.conf.inc.php';
  include 'fonction.php';
?>


<!DOCTYPE html>
<html lang="en">
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">

        <link href="https://fonts.googleapis.com/css?family=Open+Sans:300,400,600" rel="stylesheet">
        <link rel="stylesheet" href="css/main.css">
        <link rel="shortcut icon" type="image/png" href="img/favicon.png">

        <title>Facekey &mdash; Admin Panel</title>
    </head>
    <body>
        <div>
            <div>
                <header>
                    <h1>Facekey Admin Panel</h1>
                    <h2>Access to user data</h2>
                    <a href="userview.php">Click here<svg xmlns="http://www.w3.org/2000/svg" width="8" height="14" viewBox="0 0 16 28" class="link__icon"><defs><style>.cls-1 {fill-rule: evenodd;}</style></defs><path id="Rectangle_2_copy" data-name="Rectangle 2 copy" class="cls-1" d="M14.639,15.929L5.248,26.335a2.646,2.646,0,1,1-3.741-3.741l7.74-8.577L1.479,5.409A2.646,2.646,0,0,1,5.22,1.668l9.391,10.406A2.8,2.8,0,0,1,14.639,15.929Z"></path></svg></a>
                    <h2>Access to site data</h2>
                    <a href="siteview.php">Click here</a>
                </header>
            </div>
        </div>
    </body>
</html>
