package org.example;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;

class AntPathMatcherTest {

    private AntPathMatcher matcher;

    @BeforeEach
    void setUp() {
        matcher = new AntPathMatcher();
    }

    @Test
    void match() {
        assertTrue(matcher.match("machin/truc/file.js", "machin/truc/file.js"));
    }
}