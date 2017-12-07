
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
      <h1>User</h1>
      <h2>RECHERCHE</h2>

      <form method="post" action="userview.php">
  			<input type="text" name="entre">
  			<select name="option" id="option">
  				<option value="id_user">ID_User</option>
  				<option value="name">Name</option>
          <option value="pseudo">Pseudo</option>
          <option value="mail">Mail</option>
  			</select>
  			<input type="submit" name="valid" value="Recherche">
  		</form>

      <?php
        if (isset($_POST['entre']) && isset($_POST['option'])){
          $choise = $_POST['option'];
          $search = $_POST['entre'];
          $query = "SELECT * FROM Users WHERE $choise = '$search' ORDER BY id_user;";
          //echo $query;
          echo display_table_query($query,3);
        }
        else{
          echo "<p>Veuillez renseigner tous les champs de recherche</p>\n";
      }?>

      <h2>AJOUTER DONNEES</h2>

      <a href="add.php">Add New Data</a><br/><br/>



      <!--<a href="test.php">Test</a><br/><br/> -->

      <h2>LISTE TOUT LES USERS</h2>

      <?php 
          $query = "SELECT * FROM Users ORDER BY id_user;";
          echo display_table_query($query,3);
      ?>
    </body>
</html>
