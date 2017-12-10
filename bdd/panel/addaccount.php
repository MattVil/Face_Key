<?php
    include "include.php";

    $id = $_GET['id'];

    $thisprofile="profile.php?id=$id";
    $view = "Location: $thisprofile";
    $thisedit = "./addaccount.php?id=$id";
    $exist = false;
    
    $profile = "./profile.php?id=$id";
    $name_user = get_info("Users", $id, "pseudo", "id_user");

    $champ = "id_user";
    $value = "$id";
    $exist = false;
    $_POST = array_filter($_POST);
    foreach($_POST as $k => $v){
        if(isset($_POST[$k])){
            $exist = true;

            if($k=="domain"){
                $pr = $_POST[$k];
                $new_id_site = get_info("Sites", $_POST[$k], "id_site", "domain");
                $champ = insert_request_champ("id_site",$champ);
                $value = insert_request_value($new_id_site, $value);
 
            }
            else if($k=="tag"){
                $new_id_tag = get_info("Tags", $_POST[$k], "id_tag", "name_tag");
                $champ = insert_request_champ("id_tag",$champ);
                $value = insert_request_value($new_id_tag, $value);

            }
            else{
                $champ = insert_request_champ($k,$champ);
                $value = insert_request_value($_POST[$k], $value);
            }
            /*echo "ch : " . $champ . "<br>";
            echo "val : " . $value . "<br>";
            echo "<br>";*/
            // echo "champ : $champ || value $value <br/>";
        }
    }
    if($exist){
        //echo "<br><br><br><br>final : <br>champs = " . $champ ."<br>value = " . $value . "<br>";
        add_to_table("account",$champ,$value);
        header($view);
    }
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
                    <a href="userview.php" class="gradient-2">Back</a>
                    <a href="<?php echo $profile ?>" class="gradient-2">Profile</a>
                </div>
            </div>
            <h2 class="gradient-1">Add account for <?php echo $name_user ?></h2>
  
            <form action="<?php $thisedit?>" method="post">
                <ul>
                    <li> <strong>site :</strong> <select name="domain"><?php echo get_list("Sites","domain",0)?></select></li>
                    <li> <strong>login :</strong> <input name="login"></li>
                    <li> <strong>password :</strong> <input name="password"></li>
                    <li> <strong>tag :</strong> <select name="tag"><?php echo get_list("Tags","name_tag",0)?></select></li>
                </ul>
                <div class="center">
                    <input type="submit" value="Submit">
                </div>
            </form>
        </div>
    </body>
</html>
