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
        echo "<h2>Details du site $value</h2>";
      ?>

      <h3>Infos generales</h3>

      <?php

        $result = pg_query("SELECT domain FROM Sites WHERE id_site = '$id'");
        $domain = current(pg_fetch_row($result));
        $result = pg_query("SELECT login_input FROM Sites WHERE id_site = '$id'");
        $login_input = current(pg_fetch_row($result));
        $result = pg_query("SELECT password_input FROM Sites WHERE id_site = '$id'");
        $password_input = current(pg_fetch_row($result));

        echo "<form action = \"siteview.php\" method=\"post\">";
        echo "<ul>";
          echo "<li>ID : <input type=\"text\" name=\"id\" placeholder=\"$id\"></li>";
          echo "<li>Domain : <input type=\"text\" name=\"domain\" placeholder=\"$domain\"></li>";
          echo "<li>Input field : <input type=\"text\" name=\"login\" placeholder=\"$login_input\"></li>";
          echo "<li>Password field : <input type=\"text\" name=\"password\" placeholder=\"$password_input\"></li>";
        echo "</ul>";

        echo "<input type=\"submit\" value=\"Submit\">";

        echo "</form>";

        if (isset($_POST['id']) || isset($_POST['domain']) || isset($_POST['login']) || isset($_POST['password'])){
          $query = "UPDADE sites SET id_site='".$_POST['id']."', domain='".$_POST['domain']."', login_input='".$_POST['login']."', password_input='".$_POST['password']."' WHERE id_site = '$id'";
          pg_query($query);
        }
      ?>

      <h3>Fréquence d'utilisation</h3>

      <?php
        $query = "SELECT nb_user_today, nb_user_week, nb_user_month FROM Sites WHERE id_site = '$id';";
        //echo $query;
        echo display_table_query($query);
       ?>

       <h3>Geolocalisation utilisateur</h3>

       <?php
         $query = "SELECT last_loc FROM DataAccount INNER JOIN Account ON DataAccount.id_account = Account.id_account INNER JOIN Sites ON Account.id_site = Sites.id_site WHERE sites.id_site = '$id';";
         //echo $query;
         echo display_table_query($query);
        ?>

    </body>
</html>
