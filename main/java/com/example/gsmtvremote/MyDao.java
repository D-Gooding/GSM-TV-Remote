package com.example.gsmtvremote;


import androidx.room.Dao;
import androidx.room.Insert;

@Dao

public interface MyDao
{
    @Insert
    public void addUser(tv_channel_storage user);

}
