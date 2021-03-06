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
            <div class="row vcenter nav-back">
            </div>
            <div class="row nav">
                <div class="container">
                    <div class="row vcenter">
                        <div class="col-6 ">
                            <div class="row">
                                <h1 class="left"><a class="logo" href="./index.php">Facekey Admin Panel</a></h1>
                            </div>
                        </div>
                        <div class="col-6 right">
                            <div class="row vcenter">
                                <div class="">
                                </div>
                                <div class="col-8">
                                </div>
                                <div class="col-4">
                                    <a href="./userview.php" class="nava white">Users </a>
                                    <a href="./siteview.php" class="nava white">Sites </a>

                                </div>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
            <div class="nav-post">
            </div>
      <h2 class="gradient-8">User</h2>
      <div class="row">
        <div class="col-6 col-sm-12">
          <h3>Search.</h3>
          <form class="center" method="post" action="userview.php">
            <input type="text" name="entre">
            <select name="option" id="option">
              <option value="id_user">ID_User</option>
              <option value="name">Name</option>
              <option value="pseudo">Pseudo</option>
              <option value="mail">Mail</option>
            </select>
            
            <input type="submit" name="valid" value="Recherche">
          </form>
          <div class="tablediv">
            <?php
            if (isset($_POST['entre']) && isset($_POST['option'])){
            $choise = $_POST['option'];
            $search = $_POST['entre'];
            $query = "SELECT id_user,  name, pseudo FROM Users WHERE $choise = '$search' ORDER BY id_user;";
            //echo $query;
            echo display_table_query($query,3);
            }
            else{
            echo "<p class=\"center\">Veuillez renseigner tous les champs de recherche</p>\n";
            }?>
        </div>
        </div>
        <div class="col-6 col-sm-12">
          <h3>Add data.</h3>
          <div class="center"><a href="add.php" class="gradient-2">Add New Data ></a></div>
        </div>
      </div>
      <div class="row">
        <h3>Users list.</h3>
        <div class="tablediv">
          <?php
            $query = "SELECT id_user,  name,  first_name, gender, pseudo,  mail,  language, creation_date FROM Users ORDER BY id_user;";
            echo display_table_query($query,3);
          ?>
        </div>
      </div>
    </div>
  </body>
</html>