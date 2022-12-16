package com.example.pardue_inventory;

import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.database.Cursor;
import android.os.Bundle;
import android.view.Gravity;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.GridView;
import android.widget.Toast;

public class Add extends AppCompatActivity {

    DBmenuHelper myDb;
    private Button ViewInventory, btnAddData, MenuBack;
    private EditText editName, editSupplier;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.add);
        myDb = new DBmenuHelper(this);
        editName = (EditText) findViewById(R.id.ItemLine1);
        editSupplier = (EditText) findViewById(R.id.ItemLine2);
        btnAddData = (Button) findViewById(R.id.button_add);
        ViewInventory =(Button) findViewById(R.id.viewInventory);
        MenuBack =(Button) findViewById(R.id.menuBack);
        AddData();


        //view added database entries
        ViewInventory.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Cursor cursor = myDb.getAlldata();
                if(cursor.getCount()==0){
                    Toast.makeText(Add.this, "No Entry Exists", Toast.LENGTH_SHORT).show();
                    return;
                }
                StringBuffer buffer = new StringBuffer();
                while(cursor.moveToNext()){
                    buffer.append("Id :"+cursor.getString(0)+"\n");
                    buffer.append("Name :"+cursor.getString(1)+"\n");
                    buffer.append("Supplier :"+cursor.getString(2)+"\n\n");
                }

                AlertDialog.Builder builder = new AlertDialog.Builder(Add.this);
                builder.setCancelable(true);
                builder.setTitle("Inventory Items");
                builder.setMessage(buffer.toString());
                builder.show();
            }
        });

        MenuBack.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(getApplicationContext(), CRUDmenu.class);
                startActivity(intent);
            }
        });
    }

    //Inserts data into Inventory.db
    public void AddData() {
        btnAddData.setOnClickListener(
                new View.OnClickListener() {
                    @Override
                    public void onClick(View view) {

                        boolean isInserted = myDb.insertData(editName.getText().toString(),
                                editSupplier.getText().toString());

                        //verifies if added successfully.
                        Toast toast;
                        if (isInserted = true) {
                           toast = Toast.makeText(Add.this, "Data Inserted", Toast.LENGTH_SHORT);
                            toast.setGravity(Gravity.TOP, 0, 150);
                            toast.show();
                        } else {
                            toast = Toast.makeText(Add.this, "Data NOT Inserted", Toast.LENGTH_LONG);
                            toast.setGravity(Gravity.TOP, 0, 150);
                            toast.show();
                        }

                    }
                }


        );


        }





}