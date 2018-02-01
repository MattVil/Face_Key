package xyz.facekey.projet;

import java.io.File;
import java.util.ArrayList;
import android.Manifest;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.os.Environment;
import android.app.Activity;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.BaseAdapter;
import android.widget.CheckBox;
import android.widget.GridView;
import android.widget.ImageView;
import android.widget.Toast;


public class SortActivity extends Activity {

    private static final int PERMISSION_REQUEST_CODE = 1;
    CheckBox ch;
    File[] files;

    // image adapter to put into gridveiw
    private class ImageAdapter extends BaseAdapter {
        private Context mContext;
        ArrayList<String> itemList = new ArrayList<>();

        ImageAdapter(Context c) {
            mContext = c;
        }

        void add(String path){
            itemList.add(path);
        }

        @Override
        public int getCount() {
            return itemList.size();
        }

        @Override
        public Object getItem(int arg0) {
            // TODO Auto-generated method stub
            return null;
        }

        @Override
        public long getItemId(int position) {
            // TODO Auto-generated method stub
            return 0;
        }

        // Geting a view function
        @Override
        public View getView(int position, View convertView, ViewGroup parent) {
            ImageView iv;
            if (convertView == null) {
                iv = new ImageView(mContext);
                iv.setLayoutParams(new GridView.LayoutParams(220, 220));
                iv.setScaleType(ImageView.ScaleType.CENTER_CROP);
                iv.setPadding(8, 8, 8, 8);
            } else {
                iv = (ImageView) convertView;
            }
            Bitmap bm = bitmapfromURI(itemList.get(position), 220, 220);
            iv.setImageBitmap(bm);
            return iv;
        }

        Bitmap bitmapfromURI(String path, int reqWidth, int reqHeight) {
            Bitmap bm ;
            final BitmapFactory.Options options = new BitmapFactory.Options();
            options.inJustDecodeBounds = true;
            BitmapFactory.decodeFile(path, options);
            // Calculate inSampleSize
            options.inSampleSize = calculateInSampleSize(options, reqWidth, reqHeight);
            // Decode bitmap with inSampleSize
            options.inJustDecodeBounds = false;
            bm = BitmapFactory.decodeFile(path, options);
            return bm;
        }

        int calculateInSampleSize(BitmapFactory.Options options, int reqWidth, int reqHeight) {
            //  height and width of image
            final int height = options.outHeight;
            final int width = options.outWidth;
            int inSampleSize = 1;
            if (height > reqHeight || width > reqWidth) {
                if (width > height) {
                    inSampleSize = Math.round((float)height / (float)reqHeight);
                } else {
                    inSampleSize = Math.round((float)width / (float)reqWidth);
                }
            }
            return inSampleSize;
        }
    }


    ImageAdapter myImageAdapter;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_sort);
        // Ask Reading internal storage permission
        if(ContextCompat.checkSelfPermission(SortActivity.this,Manifest.permission.READ_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED){
            if(ActivityCompat.shouldShowRequestPermissionRationale(SortActivity.this, Manifest.permission.READ_EXTERNAL_STORAGE)){
                ActivityCompat.requestPermissions(SortActivity.this, new String[]{Manifest.permission.READ_EXTERNAL_STORAGE}, PERMISSION_REQUEST_CODE);
            }
            else{
                ActivityCompat.requestPermissions(SortActivity.this, new String[]{Manifest.permission.READ_EXTERNAL_STORAGE}, PERMISSION_REQUEST_CODE);
            }
        }
        else{
            // if OK start listImgs Function
            listImgs();
        }
    }

        // listImgs functuin
        public void listImgs(){
            // getting grid view and image adapter
            final GridView gridview = findViewById(R.id.gridview);
            myImageAdapter = new ImageAdapter(this);
            // setting up
            gridview.setAdapter(myImageAdapter);
            ch = findViewById(R.id.chb);
            // Getting custom picture folder
            String root = Environment.getExternalStorageDirectory().getAbsolutePath();
            File myDir = new File(root + "/saved_images");
            if(!myDir.exists()){
                myDir.mkdir();
            }
            // Getting folder path
            String path = myDir.toString();
            Log.d("Files", "Path: " + path);
            // listing file
            files = myDir.listFiles();
            Log.d("Files", "Size: " + files.length);
            // browsing files
            for(File f : files){
                // add to image adapter the file
                Log.d("Files", "FileName:" + f.getName());
                myImageAdapter.add(f.getAbsolutePath());
            }
            // Add a onclick listenr to grid view to delete img
            gridview.setOnItemClickListener(new AdapterView.OnItemClickListener() {
                @Override
                public void onItemClick(AdapterView<?> parent, View v,
                                        int position, long id) {
                    // if checkcbox is enabled
                    if (ch.isChecked()) {
                        // getting file from position
                        File item = files[position];
                        // deleting and checking deleting
                        boolean deleted = item.delete();
                        Log.d("Files", "Removed " + deleted);
                        // reload page to delete from the grid
                        reload();
                    }
                }
            });
        }

    // reload function, simply reload activity
    public void reload(){
        finish();
        startActivity(getIntent());
    }

    // Toast for permission checking, toast a message ton confirm or infirm permission granting
    public void onRequestPermissionsResult(int requestCode, String permissions[], int[] grantResults) {
        switch (requestCode){
            case PERMISSION_REQUEST_CODE: {
                if(grantResults.length >0 && grantResults[0] == PackageManager.PERMISSION_GRANTED){
                    if(ContextCompat.checkSelfPermission(SortActivity.this, Manifest.permission.READ_EXTERNAL_STORAGE) == PackageManager.PERMISSION_GRANTED) {
                        Toast.makeText(this, "Permision Granted ! ", Toast.LENGTH_SHORT).show();
                        listImgs();
                    }
                }
                else{
                    Toast.makeText(this, "No Permision Granted ! ", Toast.LENGTH_SHORT).show();
                    finish();

                }
            }
        }
    }



}
