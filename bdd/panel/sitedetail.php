<?php
    include "include.php";
?>

<?php
  $id = $_GET['id'];
  $thisedit = "./siteedit.php?id=$id";
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
        <div class="row vcenter">
          <div class="col-6 ">
            <h1 class="gradient-4"><a href="./index.php" style="font-weight: 900;">Facekey Admin Panel</a></h1>
          </div>
          <div class="col-6 right">
            <a href="./siteview.php" class="gradient-2">Back</a>
            <a href="<?php echo $thisedit?>" class="gradient-2">Edit</a>
          </div>
        </div>
     
        <h2 class="gradient-1"><?php echo $domain?> details</h2>
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
            <div class="tablediv">
               <?php
                $query = "SELECT DISTINCT name_tag FROM Account INNER JOIN Tags ON account.id_tag = tags.id_tag INNER JOIN sites ON account.id_site = Sites.id_site WHERE account.id_site = '$id';";
                echo display_table_query($query);
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
                src="https://maps.googleapis.com/maps/api/staticmap?size=512x512&maptype=roadmap\<?php echo loc_gmap_format($domain)?>&key=AIzaSyBKZ4bNWmhcz2LNuXuAS1OADJ0FKpMaKUE"
                alt="map"
              />
            </div>
          </div>
      </div>
    </body>
</html>
