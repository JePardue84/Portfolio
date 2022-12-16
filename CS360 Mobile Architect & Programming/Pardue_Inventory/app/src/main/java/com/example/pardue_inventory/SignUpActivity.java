package com.example.pardue_inventory;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.text.TextUtils;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

public class SignUpActivity extends AppCompatActivity {

    EditText username, password, rePassword;
    Button register, existing;
    DBloginHelper DB;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_sign_up);
        username = findViewById(R.id.createUsername);
        password = findViewById(R.id.createPassword);
        rePassword = findViewById(R.id.rePassword);
        register = findViewById(R.id.Register);
        existing = findViewById(R.id.Existing);
        DB = new DBloginHelper(this);

        register.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                String user = username.getText().toString();
                String pass = password.getText().toString();
                String repass = rePassword.getText().toString();

                if (TextUtils.isEmpty(user) || TextUtils.isEmpty(pass) || TextUtils.isEmpty(repass))
                    Toast.makeText(SignUpActivity.this, "All fields Required", Toast.LENGTH_SHORT).show();
                else {
                    if (pass.equals(repass)) {
                        Boolean checkuser = DB.checkusername(user);
                        if (checkuser == false) {
                            Boolean insert = DB.insertData(user, pass);
                            if (insert == true) {
                                Toast.makeText(SignUpActivity.this, "Registered Successfully", Toast.LENGTH_SHORT).show();
                                Intent intent = new Intent(getApplicationContext(), MainLogin.class);
                                startActivity(intent);
                            }else{
                                Toast.makeText(SignUpActivity.this, "Registration Failed", Toast.LENGTH_SHORT).show();
                            }
                        }else{
                            Toast.makeText(SignUpActivity.this, "User Already Exists", Toast.LENGTH_SHORT).show();
                            Intent intent = new Intent(getApplicationContext(), MainLogin.class);
                            startActivity(intent);
                        }
                    }else{
                        Toast.makeText(SignUpActivity.this, "Passwords do not match", Toast.LENGTH_SHORT).show();
                    }
                }

            }

        });

        existing.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(getApplicationContext(), MainLogin.class);
                startActivity(intent);
            }
        });
    }
}