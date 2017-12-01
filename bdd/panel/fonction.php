<?php
    function display_table($name_table){
        $query = 'select * from ' . $name_table;
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

?>