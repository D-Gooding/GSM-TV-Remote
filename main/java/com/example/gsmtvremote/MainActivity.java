package com.example.gsmtvremote;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

import android.Manifest;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.telephony.SmsManager;
import android.widget.Button;
import android.widget.EditText;
import android.view.View;
import android.widget.ImageButton;
import android.widget.Toast;
import android.widget.Button;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {
    private EditText editTextNumber;
    private EditText editTextMessage;

    private EditText NumPadText;

    private int volumeLevel;



    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        ActivityCompat.requestPermissions(MainActivity.this, new String[]{Manifest.permission.SEND_SMS, Manifest.permission.READ_SMS}, PackageManager.PERMISSION_GRANTED); //this ensures you get the permissions to use SMS etc

        //editTextMessage = findViewById(R.id.editText);
        //editTextNumber = findViewById(R.id.editTextNumber);


        Button button1 = findViewById(R.id.n1);
        Button button2 = findViewById(R.id.n2);
        Button button3 = findViewById(R.id.n3);
        Button button4 = findViewById(R.id.n4);
        Button button5 = findViewById(R.id.n5);
        Button button6 = findViewById(R.id.n6);
        Button button7 = findViewById(R.id.n7);
        Button button8 = findViewById(R.id.n8);
        Button button9 = findViewById(R.id.n9);
        Button button0 = findViewById(R.id.n0);

        Button buttonClear = findViewById(R.id.clear);

        Button volumeUp = findViewById(R.id.volPlus);
        Button volumeDown = findViewById(R.id.volNeg);

        Button online = findViewById(R.id.CheckOnline);

        ImageButton buttonBBC1 = findViewById(R.id.BBCButton);
        ImageButton buttonitv3 = findViewById(R.id.itv3Button);
        ImageButton buttonOn = findViewById(R.id.OnButton);

        button1.setOnClickListener(this);
        button2.setOnClickListener(this);
        button3.setOnClickListener(this);
        button4.setOnClickListener(this);
        button5.setOnClickListener(this);
        button6.setOnClickListener(this);
        button7.setOnClickListener(this);
        button8.setOnClickListener(this);
        button9.setOnClickListener(this);
        button0.setOnClickListener(this);

        buttonClear.setOnClickListener(this);

        volumeUp.setOnClickListener(this);
        volumeDown.setOnClickListener(this);

        online.setOnClickListener(this);

        buttonBBC1.setOnClickListener(this);
        buttonitv3.setOnClickListener(this);
        buttonOn.setOnClickListener(this);



    }

    public void sendSMS(View view){
        editTextMessage = findViewById(R.id.editTextMessage);
        String message = editTextMessage.getText().toString();
        String number = ("***********");
        String arr[] = message.split(" ", 2);
        String firstWord = arr[0];   //the
        if (message.isEmpty()){
            Toast.makeText(this,"Please enter a TV channel", Toast.LENGTH_SHORT).show();
            return;
        }
        else if (message.equals("ON") || message.equals("TEXT")){
            message = ("TV "+message);
        }
        else if (firstWord.equals("Volume")){
            if(volumeLevel > 0){
                message = ("TV VOL +"+volumeLevel);
            }
            message = ("TV VOL "+volumeLevel);
        }
        else{
            message = ("TV CHL "+message);
        }

        SmsManager mySmsManger = SmsManager.getDefault();
        mySmsManger.sendTextMessage(number,null,message,null,null);
        Toast.makeText(this,"Sent to TV", Toast.LENGTH_SHORT).show();
        final EditText editTextMessage = (EditText) findViewById(R.id.editTextMessage);
        editTextMessage.setText("");
        volumeLevel = 0;


    }

    @Override
    public void onClick(View v) {
        final EditText editTextMessage = (EditText) findViewById(R.id.editTextMessage);
        switch (v.getId()){

            case R.id.n1:
                //Toast.makeText(this,"Button 1 clicked", Toast.LENGTH_SHORT).show();
                editTextMessage.setText(editTextMessage.getText().toString() + "1");

                break;
            case R.id.n2:
                //Toast.makeText(this,"Button 2 clicked", Toast.LENGTH_SHORT).show();
                editTextMessage.setText(editTextMessage.getText().toString() + "2");
                break;
            case R.id.n3:
                //Toast.makeText(this,"Button 3 clicked", Toast.LENGTH_SHORT).show();
                editTextMessage.setText(editTextMessage.getText().toString() + "3");
                break;
            case R.id.n4:
                //Toast.makeText(this,"Button 4 clicked", Toast.LENGTH_SHORT).show();
                editTextMessage.setText(editTextMessage.getText().toString() + "4");
                break;
            case R.id.n5:
                //Toast.makeText(this,"Button 5 clicked", Toast.LENGTH_SHORT).show();
                editTextMessage.setText(editTextMessage.getText().toString() + "5");
                break;
            case R.id.n6:
                //Toast.makeText(this,"Button 6 clicked", Toast.LENGTH_SHORT).show();
                editTextMessage.setText(editTextMessage.getText().toString() + "6");
                break;
            case R.id.n7:
                //Toast.makeText(this,"Button 7 clicked", Toast.LENGTH_SHORT).show();
                editTextMessage.setText(editTextMessage.getText().toString() + "7");
                break;
            case R.id.n8:
                //Toast.makeText(this,"Button 8 clicked", Toast.LENGTH_SHORT).show();
                editTextMessage.setText(editTextMessage.getText().toString() + "8");
                break;
            case R.id.n9:
                //Toast.makeText(this,"Button 9 clicked", Toast.LENGTH_SHORT).show();
                editTextMessage.setText(editTextMessage.getText().toString() + "9");
                break;

            case R.id.n0:
                //Toast.makeText(this,"Button 0 clicked", Toast.LENGTH_SHORT).show();
                editTextMessage.setText(editTextMessage.getText().toString() + "0");
                break;

            case R.id.BBCButton:
                //Toast.makeText(this,"BBC 1 clicked", Toast.LENGTH_SHORT).show();
                editTextMessage.setText("1");
                break;

            case R.id.itv3Button:
                //Toast.makeText(this,"itv 3 clicked", Toast.LENGTH_SHORT).show();
                editTextMessage.setText("10");
                break;

            case R.id.OnButton:
                Toast.makeText(this,"TV On", Toast.LENGTH_SHORT).show();
                editTextMessage.setText("ON");
                break;

            case R.id.clear:
                Toast.makeText(this,"Cleared", Toast.LENGTH_SHORT).show();
                editTextMessage.setText("");
                volumeLevel = 0;
                break;

            case R.id.volPlus:
                volumeLevel = volumeLevel + 5;
                editTextMessage.setText("Volume " + Integer.toString(volumeLevel));
                break;

            case R.id.volNeg:
                volumeLevel = volumeLevel - 5;
                editTextMessage.setText("Volume " + Integer.toString(volumeLevel));
                break;

            case R.id.CheckOnline:
                editTextMessage.setText("TEXT");

        }

    }
}