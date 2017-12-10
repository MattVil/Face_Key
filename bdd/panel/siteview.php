<?php
    include "include.php";
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

      <div class="center"><a href="./index.php">Back</a></div>

      <h1>SITE</h1>

      <h2>RECHERCHE</h2>

      <form method="post" action="siteview.php" class="center">
  			<input type="text" name="entre">
  			<select name="option" id="option">
  				<option value="id_site">ID_site</option>
  				<option value="domain">Domain</option>
  			</select>
  			<input type="submit" name="valid" value="Recherche">
  		</form>

      <?php
        if (isset($_POST['entre']) && isset($_POST['option'])){
          $choise = $_POST['option'];
          $search = $_POST['entre'];
          $query = "SELECT id_site, domain, nb_user_today FROM Sites WHERE $choise = '$search';";
          //echo $query;
<<<<<<< HEAD
          echo display_table_query($query,4);
=======
          echo display_table_query($query, 4);
>>>>>>> b7e2139f1eed66228f0251ade3676a030a8b65fa
        }
        else{
          echo "<p class=\"center\">Veuillez renseigner tous les champs de recherche</p>\n";
      }?>

      <h2>AJOUTER DONNEES</h2>

      <div class="center"><a href="addSite.php">Add New Data</a></div>

      <h2>LISTE TOUT LES SITES</h2>

      <?php echo display_table_query("SELECT id_site, domain, nb_user_today FROM Sites ORDER BY id_site;",4); ?>

    </body>
</html>
