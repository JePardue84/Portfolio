package com.example.pardue_inventory;

import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.database.Cursor;
import android.os.Bundle;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.GridView;
import android.widget.Toast;

public class Update extends AppCompatActivity {
    private Button btnbackView, btnupdate, view;
    private EditText IDupdate,NAMEupdate,SUPPLIERupdate;
    DBmenuHelper db;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.update);
        view = (Button) findViewById(R.id.buttonViewALL);
        btnbackView = (Button) findViewById(R.id.Backview);
        btnupdate = (Button) findViewById(R.id.updateButton);
        IDupdate = (EditText) findViewById(R.id.editTextID);
        NAMEupdate = (EditText) findViewById(R.id.editTextNAME);
        SUPPLIERupdate = (EditText) findViewById(R.id.editTextSUPPLIER);

        db = new DBmenuHelper(this);

        btnbackView.setOnClickListener(new android.view.View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(Update.this, CRUDmenu.class);
                startActivity(intent);
            }
        });

        btnupdate.setOnClickListener(
                new View.OnClickListener() {
                    @Override
                    public void onClick(View view) {
                        boolean isUpdate = db.updateData(IDupdate.getText().toString(),
                                NAMEupdate.getText().toString(),SUPPLIERupdate.getText().toString());
                        if (isUpdate == true)
                            Toast.makeText(Update.this, "Data Updated", Toast.LENGTH_LONG).show();
                        else
                            Toast.makeText(Update.this, "Data Not Updated", Toast.LENGTH_LONG).show();
                    }
                }
        );

        view.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Cursor cursor = db.getAlldata();
                if(cursor.getCount()==0){
                    Toast.makeText(Update.this, "No Entry Exists", Toast.LENGTH_SHORT).show();
                    return;
                }
                StringBuffer buffer = new StringBuffer();
                while(cursor.moveToNext()){
                    buffer.append("Id :"+cursor.getString(0)+"\n");
                    buffer.append("Name :"+cursor.getString(1)+"\n");
                    buffer.append("Supplier :"+cursor.getString(2)+"\n\n");
                }

                AlertDialog.Builder builder = new AlertDialog.Builder(Update.this);
                builder.setCancelable(true);
                builder.setTitle("Inventory Items");
                builder.setMessage(buffer.toString());
                builder.show();
            }
        });

    }



    }

