package xyz.facekey.projet;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.EditText;
import android.widget.Toast;


public class MainActivity extends AppCompatActivity {

    // password / logi,
    public static String[][] login = {{"admin",""},{"Login","p@ssw0rd"},{"louis.lharidon@etu.u-cergy.fr", "azerty"}};

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }

    //Submit fonction to switch activity
    public void submit(View view) {
        // new intent
        Intent intent = new Intent(this, SecondActivity.class);
        // getting editTexts content
        EditText eTlog = (EditText) findViewById(R.id.log);
        EditText eTpass = (EditText) findViewById(R.id.pass);
        // Converting to string
        String name = eTlog.getText().toString();
        String pass = eTpass.getText().toString();
        // Checking combinaison
        boolean found = false;
        for(String[]s : login) {
            if ((name.equals(s[0])) && (pass.equals(s[1]))) {
                // login ok : starting intent
                found = true;
                startActivity(intent);
            }
        }
        if (!found){
            Toast.makeText(this.getApplicationContext(), "failed to connect try | p@ssw0rd", Toast.LENGTH_SHORT).show();
        }
    }
}
