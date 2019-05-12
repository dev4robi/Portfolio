package com.ghasix.datas.converter;

import javax.persistence.AttributeConverter;

import com.ghasix.datas.enums.UsersStatus;

public class UsersStatusConverter implements AttributeConverter<UsersStatus, String> {

    @Override
    public String convertToDatabaseColumn(UsersStatus attribute) {
        return attribute.getValue();
    }

    @Override
    public UsersStatus convertToEntityAttribute(String dbData) {
        return UsersStatus.valueOf(dbData);
    }
}