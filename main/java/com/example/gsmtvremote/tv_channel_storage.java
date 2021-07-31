package com.example.gsmtvremote;


import androidx.room.ColumnInfo;
import androidx.room.Entity;
import androidx.room.PrimaryKey;

@Entity(tableName = "TVChannels")
public class tv_channel_storage {

    @PrimaryKey
    private int ChannelNumber;

    @ColumnInfo(name = "channel_name")
    private String ChannelName;

    public int getChannelNumber() {
        return ChannelNumber;
    }

    public void setChannelNumber(int channelNumber) {
        ChannelNumber = channelNumber;
    }

    public String getChannelName() {
        return ChannelName;
    }

    public void setChannelName(String channelName) {
        ChannelName = channelName;
    }
}
