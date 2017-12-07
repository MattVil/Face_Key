<?php
  // include 'local.postgre.conf.php';
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
        $thisedit = "./siteedit.php?id=$id";
        $domain = get_info("Sites", $id, "domain", "id_site");
        $login_input = get_info("Sites", $id, "login_input", "id_site");
        $password_input = get_info("Sites", $id, "password_input", "id_site");

      ?>
      <div class="center">
        <a href="./siteview.php">Back</a>
        <a href="<?php echo $thisedit?>">Edit</a>
      </div>

      <h1>Details du site <?php echo $domain?></h1>

      <h2>Infos generales</h2>

        <ul>
          <li>ID : <?php echo $id?></li>
          <li>Domain : <?php echo $domain?></li>
          <li>Input field : <?php echo $login_input?></li>
          <li>Password field : <?php echo $password_input?></li>
        </ul>




      <h2>Fréquence d'utilisation</h2>

      <?php
        $query = "SELECT nb_user_today, nb_user_week, nb_user_month FROM Sites WHERE id_site = '$id';";
        //echo $query;
        echo display_table_query($query);
       ?>

       <h2>Type d'utilisation</h2>

       <?php
        $query = "SELECT DISTINCT name_tag FROM Account INNER JOIN Tags ON account.id_tag = tags.id_tag INNER JOIN sites ON account.id_site = Sites.id_site WHERE account.id_site = '$id';";
        echo display_table_query($query);
        ?>

       <h2>Geolocalisation utilisateur</h2>

       <?php
         $query = "SELECT last_loc FROM DataAccount INNER JOIN Account ON DataAccount.id_account = Account.id_account INNER JOIN Sites ON Account.id_site = Sites.id_site WHERE sites.id_site = '$id';";
         //echo $query;
         echo display_table_query($query);
        ?>

        <p class="center"><img
          src="https://maps.googleapis.com/maps/api/staticmap?size=512x512&maptype=roadmap\<?php echo loc_gmap_format($domain)?>&key=AIzaSyBKZ4bNWmhcz2LNuXuAS1OADJ0FKpMaKUE"
          alt="map"
        /></p>

    </body>
</html>
