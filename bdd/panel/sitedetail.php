<?php
  //include 'local.postgre.conf.php';
  include 'postgresql.conf.inc.php';
  include 'fonction.php';
?>

<!DOCTYPE html>
<html lang="en">
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">

        <link rel="stylesheet" href="css/main.css">
        <link rel="shortcut icon" type="image/png" href="img/favicon.png">

        <title>Facekey &mdash; Admin Panel</title>
    </head>

    <body>
      <?php
        $id = $_GET['id'];

        $name = pg_query("SELECT domain FROM Sites WHERE id_site = '$id'");
        $value = current(pg_fetch_row($name));
        echo "<h3>Details du site $value</h3>";



      ?>

    </body>
</html>
