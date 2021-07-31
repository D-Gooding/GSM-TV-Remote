package com.example.gsmtvremote;

import androidx.room.Database;
import androidx.room.RoomDatabase;


@Database(entities = {tv_channel_storage.class},version = 1)
public abstract class TVChannelDataBase extends RoomDatabase
{
    public abstract MyDao myDao();
}
