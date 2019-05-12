package com.ghasix.datas.domain;

import org.springframework.data.jpa.repository.JpaRepository;

public interface UsersRepository extends JpaRepository<Users, Long> {
    
    public Users findByEmail(String email);
}