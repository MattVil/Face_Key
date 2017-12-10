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
      <div class="container">
        <div class="row vcenter">
          <div class="col-6 ">
            <h1 class="gradient-4"><a href="./index.php" style="font-weight: 900;">Facekey Admin Panel</a></h1>
          </div>
          <div class="col-6 right">
            <a href="./index.php" class="gradient-2">back </a>
          </div>
        </div>
        <h2 class="gradient-1">Sites</h2>
        <div class="row vcenter">
          <div class="col-6 col-sm-12">
            <h3>Search.</h3>
            <form method="post" action="siteview.php" class="center">
        			<input type="text" name="entre">
        			<select name="option" id="option">
        				<option value="id_site">ID_site</option>
        				<option value="domain">Domain</option>
        			</select>
        			<input type="submit" name="valid" value="Recherche">
        		</form>
            <div class="tablediv">
              <?php
                if (isset($_POST['entre']) && isset($_POST['option'])){
                  $choise = $_POST['option'];
                  $search = $_POST['entre'];
                  $query = "SELECT id_site, domain, nb_user_today FROM Sites WHERE $choise = '$search';";
                  //echo $query;
                  echo display_table_query($query, 4);
                }
                else{
                  echo "<p class=\"center\">Veuillez renseigner tous les champs de recherche</p>\n";
              }?>
            </div>
          </div>
        <div class="col-6 col-sm-12">
          <h3>Add data.</h3>
          <div class="center"><a href="addSite.php" class="gradient-2">Add New Data ></a></div>
        </div>
      </div>
      <div class="row">
        <h3>List sites.</h3>
        <div class="tablediv">
          <?php echo display_table_query("SELECT id_site, domain, nb_user_today FROM Sites ORDER BY id_site;",4); ?>
        </div>
      </div>
    </body>
</html>
