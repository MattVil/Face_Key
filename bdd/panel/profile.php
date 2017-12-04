<?php
  	include 'local.postgre.conf.php';
  	//include 'postgresql.conf.inc.php'; 
  	include 'fonction.php';


	$id = $_GET['id']; 
	$edit = "/edit.php?id=$id";
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
    <a href="userview.php">Back</a>
    <br/>
    <a href="<?php echo $edit ?>">Edit</a>
	<br/><br/>

	<ul>
		<li> name : <?php echo get_info("users", $id, "name") ?></li>
		<li> first Name : <?php echo get_info("users", $id, "first_name") ?></li>
		<li> pseudo : <?php echo get_info("users", $id, "pseudo") ?></li>
		<li> gender : <?php echo get_info("users", $id, "gender") ?></li>
		<li> mail : <?php echo get_info("users", $id, "mail") ?></li>
		<li> Face Key password : <?php echo get_info("users", $id, "password") ?></li>
		<li> creation date : <?php echo get_info("users", $id, "creation_date") ?></li>
		<li> language : <?php echo get_info("users", $id, "language") ?></li>

		<h2>CO 1</h2>
		<? 
		function display_profil_table(){
        $query = "SELECT Users.id_user, name, pseudo, Account.id_site, id_account,login, Account.password FROM  Users INNER JOIN Account ON Users.id_user=Account.id_user";       
		$result = pg_query($query);
        $i = 0;
        $char = '<table><thead><tr>';
        while ($i < pg_num_fields($result))
        {
            $fieldName = pg_field_name($result, $i);
             $char .= '<td>' . $fieldName . '</td>';
            $i = $i + 1;
        }
        $char .= '</tr></thead><tbody>';
        $i = 0;
        while ($row = pg_fetch_row($result))
        {
            $id = current($row);
            $char .= '<tr>';
            $count = count($row);
            $j = 0;
            while ($j < $count)
            {
                $c_row = current($row);
                $char .= '<td>' . $c_row . '</td>';
                next($row);
                $j = $j + 1;
            }
            $char .= '</tr>';
            $i = $i + 1;
        }

        pg_free_result($result);
        $char .= '</tbody></table>';
        return $char;
    }


		echo display_profil_table(); ?>
	

	</ul>
    
    </body>
</html>