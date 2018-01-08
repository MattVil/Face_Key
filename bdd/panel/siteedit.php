<?php
    include "include.php";

    $id = $_GET['id'];
    $profile = "./sitedetail.php?id=$id";
    $thisprofile = "sitedetail.php?id=$id";

    $view = "Location: $profile";
    $thisedit = "./siteedit.php?id=$id";
    $thisdelete = "./sitedelete.php?id=$id";
    $exist = false;

    $_POST = array_filter($_POST);
    foreach($_POST as $k => $v){
        if(isset($_POST[$k])){
            $exist = true;
            edit_table("Sites",$id,$k,$_POST[$k],"id_site");
        }
    }
    if($exist){
        header($view);
    }

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
                                    <a href="<?php echo $profile ?>" class="gradient-22 nava">Check</a>
                                    <!-- <a href="<?php //echo $thisdelete ?>" class="gradient-22 nava">Delete</a> -->
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
            <h2 class="gradient-8">Edit of <?php echo $domain ?></h2>

            <form action="<?php $thisedit ?>" method="post">
                 <ul>
                    <li> <strong>Id :</strong> <input type="text" name="id_site" placeholder="<?php echo  $id?>"/></li>
                    <li> <strong>Domain :</strong> <input type="text" name="domain" placeholder="<?php echo $domain?>"/></li>
                    <li> <strong>Input field :</strong> <input type="text" name="login_input" placeholder="<?php echo $login_input ?>"/></li>
                    <li> <strong>Password field :</strong> <input type="text" name="password_input" placeholder="<?php echo $password_input?>"/></li>
                </ul>
                <div class="center">
                    <input type="submit" value="Submit">
                </div>
            </form>
        </div>
    </body>
</html>
