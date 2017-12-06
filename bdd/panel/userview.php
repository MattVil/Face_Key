
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

        <link rel="stylesheet" href="css/main.css">
        <link rel="shortcut icon" type="image/png" href="img/favicon.png">

        <title>Facekey &mdash; Admin Panel</title>
    </head>
    <body>

      <a href="./index.php">Back<a>

      <h3>RECHERCHE</h3>

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
          echo display_table_query($query);
        }
        else{
          echo "<p>Veuillez renseigner tous les champs de recherche</p>\n";
      }?>

      <h3>AJOUTER DONNEES</h3>

      <a href="add.php">Add New Data</a><br/><br/>



      <!--<a href="test.php">Test</a><br/><br/> -->

      <h3>LISTE TOUT LES USERS</h3>

      <?php echo display_table("users"); ?>
    </body>
</html>
