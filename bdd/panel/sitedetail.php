<?php
    include "include.php";
?>

<?php
  $id = $_GET['id'];
  $thisedit = "./siteedit.php?id=$id";
  $thisdelete = "./sitedelete.php?id=$id";
  $domain = get_info("Sites", $id, "domain", "id_site");
  $login_input = get_info("Sites", $id, "login_input", "id_site");
  $password_input = get_info("Sites", $id, "password_input", "id_site");

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
                                  <a href="<?php echo $thisedit ?>" class="gradient-22 nava">Edit</a>
                                  <!-- <a href="<?php // echo $thisdelete ?>" class="gradient-22 nava">Delete</a> -->
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
        <h2 class="gradient-8"><?php echo $domain?> details</h2>
        <div class="row vcenter">
          <div class="col-6 col-sm-12">
            <h3>Informations.</h3>
              <ul class="left">
                <li><strong>ID :</strong> <?php echo $id?></li>
                <li><strong>Domain :</strong> <?php echo $domain?></li>
                <li><strong>Input field :</strong> <?php echo $login_input?></li>
                <li><strong>Password field :</strong> <?php echo $password_input?></li>
              </ul>
          </div>
          <div class="col-6 col-sm-12">
            <h3>Use type.</h3>
            <div class="row">
               <?php
                echo display_tag($id);
                ?>
            </div>
          </div>
        </div>
        <div class="row vcenter">
          <div class="col-6 col-sm-12">
            <h3>Use frequency.</h3>
            <div class="tablediv">
              <?php
                $query = "SELECT nb_user_today, nb_user_week, nb_user_month FROM Sites WHERE id_site = '$id';";
                //echo $query;
                echo display_table_query($query);
               ?>
            </div>
          </div>
          <div class="col-6 col-sm-12">
             <h3>Users's eolocalisation.</h3>
             <div class="tablediv">
               <?php
                 $query = "SELECT last_loc FROM DataAccount INNER JOIN Account ON DataAccount.id_account = Account.id_account INNER JOIN Sites ON Account.id_site = Sites.id_site WHERE sites.id_site = '$id';";
                 //echo $query;
                 echo display_table_query($query);
                ?>
              </div>
            </div>
          </div>
          <div class="row">
            <div class="center">
              <img
                src="https://maps.googleapis.com/maps/api/staticmap?size=2048x512&maptype=roadmap\<?php echo loc_gmap_format($domain)?>&key=AIzaSyBKZ4bNWmhcz2LNuXuAS1OADJ0FKpMaKUE"
                alt="map"
              />
            </div>
          </div>
      </div>
    </body>
</html>
