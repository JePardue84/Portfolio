package com.example.pardue_inventory;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.GridView;
import android.widget.Toast;

public class MainLogin extends AppCompatActivity {

    EditText username, password;
    Button btnlogin;
    Button btnsignin;
    DBloginHelper DB;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main_login);
        username = (EditText) findViewById(R.id.username1);
        password = (EditText) findViewById(R.id.password1);
        btnlogin = (Button) findViewById(R.id.btnsignin1);
        btnsignin = (Button) findViewById(R.id.SignUp);
        DB = new DBloginHelper(this);
        btnlogin.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                String user = username.getText().toString();
                String pass = password.getText().toString();

                if (user.equals("") || pass.equals(""))
                    Toast.makeText(MainLogin.this, "Please enter all the fields", Toast.LENGTH_SHORT).show();
                else {
                    Boolean checkuserpass = DB.checkusernamepassword(user, pass);
                    if (checkuserpass == true) {
                        Toast.makeText(MainLogin.this, "Sign in successful", Toast.LENGTH_SHORT).show();
                        Intent intent = new Intent(getApplicationContext(), CRUDmenu.class);
                        startActivity(intent);
                    } else {
                        Toast.makeText(MainLogin.this, "Invalid Credentials", Toast.LENGTH_SHORT).show();
                    }
                }
            }
        });

        btnsignin.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(getApplicationContext(), SignUpActivity.class);
                startActivity(intent);
            }
        });
    }
}

