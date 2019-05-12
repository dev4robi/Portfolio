package com.ghasix.domain;

import static org.hamcrest.CoreMatchers.is;
import static org.junit.Assert.assertThat;

import java.util.List;

import com.ghasix.datas.domain.Commutes;
import com.ghasix.datas.domain.CommutesRepository;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.test.context.junit4.SpringRunner;

@RunWith(SpringRunner.class)
@SpringBootTest
public class CommutesRepositoryTests {

    @Autowired
    CommutesRepository commutesRepository;

    @Test
    public void testCommutesRepository() {
        // Create
        Commutes commutes = Commutes.builder()
                            .commuteCompanyName("KSNET")
                            .checkInTime(System.currentTimeMillis())
                            .memo("오전 반차로 늦게 출근.").build();
        commutesRepository.save(commutes);

        // Read
        List<Commutes> commutesList = commutesRepository.findAll();

        Commutes ksnetCommutes = commutesList.get(0);
        assertThat(ksnetCommutes.getCommuteCompanyName(), is("KSNET"));
    }
}