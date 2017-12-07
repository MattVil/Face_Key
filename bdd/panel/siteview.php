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

      <a href="./index.php">Back<a>
                  <h1>SITE</h1> 

      <h2>RECHERCHE</h2>

      <form method="post" action="siteview.php">
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
          echo display_table_query($query);
        }
        else{
          echo "<p>Veuillez renseigner tous les champs de recherche</p>\n";
      }?>

      <h2>AJOUTER DONNEES</h2>

      <a href="addSite.php">Add New Data</a><br/><br/>

      <h2>LISTE TOUT LES SITES</h2>

      <?php echo display_table_query("SELECT id_site, domain, nb_user_today FROM Sites;",4); ?>

    </body>
</html>
