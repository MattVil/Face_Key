<?php
    function display_table($name_table){
        // $query = "SELECT * FROM  $name_table";
        $query = "select * from  $name_table ORDER BY id_user";
        $result = pg_query($query);
        $i = 0;
        $char = '<table><thead><tr>';
        while ($i < pg_num_fields($result))
        {
            $fieldName = pg_field_name($result, $i);
             $char .= '<td>' . $fieldName . '</td>';
            $i = $i + 1;
        }
        $char .=  "<td>Update</td>";
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
            $char.= "<td><a href=\"profile.php?id=$id\">Check</a> | <a href=\"edit.php?id=$id\">Edit</a> | <a href=\"delete.php?id=" . $id . "\" onClick=\"return confirm('Are you sure you want to delete?')\">Delete</a></td>";
            $char .= '</tr>';
            $i = $i + 1;
        }

        pg_free_result($result);
        $char .= '</tbody></table>';
        return $char;
    }



    function insert_request_champ($champ, $prev_champ){
        if($prev_champ){
            $result = "$prev_champ,$champ";
        }
        else{
            $result = "$champ";
        }
        return $result;
    }
    function insert_request_value($value, $prev_value){
        if($prev_value){
            $result = "$prev_value,'$value'";
        }
        else{
            $result = "'$value'";
        }
        return $result;
    }

    function add_to_table($table, $champ, $value){
        $query = "INSERT INTO $table($champ) VALUES($value)";
        $result = pg_query($query);
    }
    function edit_table($table, $id ,$champ, $value){
        $query = "UPDATE  $table  SET $champ = '$value' WHERE id_user =$id";
        $result = pg_query($query);
    }

    function delete_line($table, $id){
        $query = "DELETE FROM $table WHERE id_user=$id";
        $result = pg_query($query);
    }

    function get_info($table, $id, $champ){
        $query = "SELECT $champ FROM $table WHERE id_user=$id";
        $result = pg_query($query);
        $value = current(pg_fetch_row($result));
        return $value;
    }

    function real_get_info($table, $id, $champ, $column){
        $query = "SELECT $champ FROM $table WHERE $column = $id";
        $result = pg_query($query);
        $value = current(pg_fetch_row($result));
        return $value;
    }

    function disp_column_option($name_table, $column){
        $char = '<select name="exo2">';
        $query = 'select '  . $column . ' from ' . $name_table;
        $result = pg_query($query);
        $i = 0;
        $fieldName = pg_field_name($result, $i);
        $char .= '<optgroup label="' . $fieldName . '">';
        $i = 0;
        while ($row = pg_fetch_row($result))
        {
            $char .= '<option value="'. current($row) . '">' . current($row) . '</option>';
            $i = $i + 1;
        }
        pg_free_result($result);
        $char .= '</optgroup></select><input type="submit"/>';
        return $char;
    }

    function disp_table_names(){
        $char = '<select name="exo3">';
        $result = pg_list_tables();
        $i = 0;
        $fieldName = pg_field_name($result, $i);
        $char .= '<optgroup label="' . $fieldName . '">';
        $i = 0;
        while ($row = pg_fetch_row($result))
        {
            $char .= '<option value="'. current($row) . '">' . current($row) . '</option>';
            $i = $i + 1;
        }
        pg_free_result($result);
        $char .= '</optgroup></select><input type="submit"/>';
        return $char;
    }

    function pg_list_tables()
    {
        $sql = "select relname from pg_stat_user_tables order by relname;";
        return pg_query($sql);
    }

    function disp_all()
    {
        $char = '';
        $result = pg_list_tables();
        $i = 0;
        $fieldName = pg_field_name($result, $i);
        while ($row = pg_fetch_row($result))
        {
            $char.= "<h2>" . current($row) . "</h2>";
            $char.= display_table(current($row));
            $i = $i + 1;
        }
        pg_free_result($result);
        return $char;
    }

    function createGraph($user){

        $path = "./users/$user/freq.csv";
        $day = array();
        $nb_conn = array();

        //lecture CSV
        if (file_exists($path)){
          if (($handle = fopen($path, "r")) !== FALSE) {
              while (($data = fgetcsv($handle, 10000, ";")) !== FALSE) {
                  array_push($nb_conn, $data[1]);
                  array_push($day, $data[0]);
                  // echo "nb con : " . $data[0] . " | day : " . $data[1] . "<br/>";
              }
              fclose($handle);

              $width = 600;
              $height = 300;

              $graph = new Graph($width,$height,'auto');
              $graph->SetScale("intlin");

              $graph->SetShadow();
              $graph->SetMargin(40,30,20,40);

              $b1plot = new BarPlot($nb_conn);
              $b1plot->SetFillColor('orange');

              $graph->Add($b1plot);



              $b1plot->SetColor("white");

              $graph->title->Set('Number of connection per day until the creations');
              $graph->xaxis->title->Set('Day number');
              $graph->yaxis->title->Set('Number of connections');

              $graph->title->SetFont(FF_FONT1,FS_BOLD);
              $graph->yaxis->title->SetFont(FF_FONT1,FS_BOLD);
              $graph->xaxis->title->SetFont(FF_FONT1,FS_BOLD);


              $gdImgHandler = $graph->Stroke(_IMG_HANDLER);
              $fileName = "./users/$user/graphFreq.png";

              $graph->img->Stream($fileName);

          }
          else{
            echo "<p><strong>There is obviously a problem here</strong></p>";
          }
        }
        else{
          echo "<p><strong>There is no value for this user</strong></p>";
        }
        //fclose($handle);
    }

    function display_table_query($query, $flag=0){
            //echo $query;
            $id = 0;
            $result = pg_query($query);
            $i = 0;
            $char = '<table><thead><tr>';
            while ($i < pg_num_fields($result))
            {
                $fieldName = pg_field_name($result, $i);
                 $char .= '<th>' . $fieldName . '</th>';
                $i = $i + 1;
            }
            $char .= '<th>Option</th></tr></thead><tbody>';
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
                  if ($flag == 1 || $flag == 2){
                    if ($j == 0)
                      $id = $c_row;
                  }
                  $char .= '<td>' . $c_row . '</td>';
                  next($row);
                  $j = $j + 1;
              }
              if ($flag == 1) //Account
                $char.= '<td><a href="account.php?id='.$id.'">Edit</a></td>';
              if ($flag == 2) //PaymentAccount
                $char.= '<td><a href="paccount.php?id='.$id.'">Edit</a></td>';
              $char .= '</tr>';
              $i = $i + 1;
            }

            pg_free_result($result);
            $char .= '</tbody></table>';
            return $char;
        }

        function loc_gmap_format($domain){
          $query = "SELECT last_loc
          FROM DataAccount
          INNER JOIN Account
            ON DataAccount.id_account = Account.id_account
          INNER JOIN Sites
            ON Account.id_site = Sites.id_site
          WHERE domain = '$domain'";

          $result = pg_query($query);
          $char = " ";

          while ($row = pg_fetch_row($result))
          {
              $char .="&markers=size:mid%7Ccolor:red%7c";
              $loc = current($row);
              $count = count($row);
              $j = 0;
              $char .= "$loc";
              next($row);
              $char .= "";
          }
          pg_free_result($result);
          $arr = array("°" => ".", " " => "");
          $char = strtr($char,$arr);
          return $char;

        }

?>
