package xyz.facekey.projet;

import android.Manifest;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.os.Environment;
import android.provider.MediaStore;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;

public class SecondActivity extends AppCompatActivity {

    private static final int PERMISSION_REQUEST_CODE = 1;
    private static final int CAMERA_REQUEST = 1888;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_second);
        // checking and asking for persissions
            // CAMERA
        if( (ContextCompat.checkSelfPermission(SecondActivity.this, Manifest.permission.CAMERA) != PackageManager.PERMISSION_GRANTED)) {
            if (ActivityCompat.shouldShowRequestPermissionRationale(SecondActivity.this, Manifest.permission.CAMERA)) {
                ActivityCompat.requestPermissions(SecondActivity.this, new String[]{Manifest.permission.CAMERA}, PERMISSION_REQUEST_CODE);
            } else {
                ActivityCompat.requestPermissions(SecondActivity.this, new String[]{Manifest.permission.CAMERA}, PERMISSION_REQUEST_CODE);
            }
        }
            // WRITING ON EXTERNAL STORATE
        if( (ContextCompat.checkSelfPermission(SecondActivity.this,Manifest.permission.WRITE_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED)){
            if (ActivityCompat.shouldShowRequestPermissionRationale(SecondActivity.this, Manifest.permission.WRITE_EXTERNAL_STORAGE)){
                ActivityCompat.requestPermissions(SecondActivity.this, new String[]{Manifest.permission.CAMERA}, PERMISSION_REQUEST_CODE);
            }
            else{
                ActivityCompat.requestPermissions(SecondActivity.this, new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE}, PERMISSION_REQUEST_CODE);
            }
        }
            // INTERNET
        if( (ContextCompat.checkSelfPermission(SecondActivity.this, Manifest.permission.INTERNET) != PackageManager.PERMISSION_GRANTED)) {
            if (ActivityCompat.shouldShowRequestPermissionRationale(SecondActivity.this, Manifest.permission.CAMERA)) {
                ActivityCompat.requestPermissions(SecondActivity.this, new String[]{Manifest.permission.INTERNET}, PERMISSION_REQUEST_CODE);
            } else {
                ActivityCompat.requestPermissions(SecondActivity.this, new String[]{Manifest.permission.INTERNET}, PERMISSION_REQUEST_CODE);
            }
        }
    }

    // going to sort activity
    public void gotosort(View view) {
       Intent intent = new Intent(this, SortActivity.class);
        startActivity(intent);
    }
    // opening camera intent
    public void gotophoto(View view) {
        Intent cameraIntent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
        startActivityForResult(cameraIntent, CAMERA_REQUEST);
    }
    // starting mysql upload to server service
    public void startmysql(View view) {
        Intent intent = new Intent(this, MySQL.class);
        startService(intent);
    }

    // getting, converting and saving the image after the camera intent
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (requestCode == CAMERA_REQUEST && resultCode == this.RESULT_OK) {
            // getting photo bitmat
            Bitmap photo = (Bitmap) data.getExtras().get("data");
            FileOutputStream out = null;
            try {
                // Getting custom picture folder
                String root = Environment.getExternalStorageDirectory().getAbsolutePath();
                File myDir = new File(root + "/saved_images");
                if(!myDir.exists()) myDir.mkdir();
                Log.d("Photos", "directory " + myDir.toString());
                // Writing filename
                String fname = "img-" + System.currentTimeMillis() + ".png";
                // Creating file
                File file = new File(myDir, fname);
                Log.d("Photos", "file " + file.toString());
                try {
                    out = new FileOutputStream(file);
                    if (photo != null) {
                        // putting bitmap photo to file after png compression
                        photo.compress(Bitmap.CompressFormat.PNG, 100, out);
                    }
                    out.flush();
                } catch (Exception e) {
                    e.printStackTrace();
                }
            } finally {
                try {
                    if (out != null) {
                        out.close();
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }

        }
    }

}
