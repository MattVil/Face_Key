<?php
    include "include.php";

    $id = $_GET['id'];


    $thisaccount = "account.php?id=$id";
    $view = "Location: $thisaccount";
    $exist = false;

    $champ = "id_account";
    $value = "$id";
    $_POST = array_filter($_POST);
    foreach($_POST as $k => $v){
        if(isset($_POST[$k])){
            $exist = true;
            if($k=="pseudo"){
                $pr = $_POST[$k];
                $new_id_user = get_info("Users", $_POST[$k], "id_user", "pseudo");
                $champ = insert_request_champ("id_receiver",$champ);
                $value = insert_request_value($new_id_user, $value);
 
            }
            else{
                $champ = insert_request_champ($k,$champ);
                $value = insert_request_value($_POST[$k], $value);
            }
            // echo "ch : " . $champ . "<br>";
            // echo "val : " . $value . "<br>";
            // echo "<br>";
            // echo "champ : $champ || value $value <br/>";
        }
    }
    if($exist){
        // echo "<br><br><br><br>final : <br>champs = " . $champ ."<br>value = " . $value . "<br>";
        add_to_table("SharedAccount",$champ,$value);
        header($view);
    }


    $id_site = get_info("account", $id, "id_site", "id_account");
    $domain = get_info("Sites", $id_site, "domain", "id_site");
    $id_user = get_info("account", $id, "id_user", "id_account");
    $pseudo = get_info("Users", $id_user, "pseudo", "id_user") ;
    

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
                                    <a href="<?php echo $thisaccount ?>" class="gradient-22 nava">Connexion</a>
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
            <h2 class="gradient-8">Share <?php echo $domain?> of <?php echo $pseudo?></h2>
            <form action="<?php $thisedit?>" method="post">
                <ul>
                    <li> <strong>Share with :</strong> <select name="pseudo"><?php echo get_list("Users","pseudo",0)?></select></li>
                    <li> <strong>Until :</strong> <input name="expiration_date" placeholder="yyyy-mm-dd"/></li>
                </ul>
                <div class="center">
                    <input type="submit" value="Submit">
                </div>
            </form>
        </div>
    </body>
</html>
