package org.example;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.springframework.util.AntPathMatcher;

import java.util.Map;

import static org.junit.jupiter.api.Assertions.*;

class AntPathMatcherFunctionalTest {

    private AntPathMatcher matcher;

    @BeforeEach
    void setUp() {
        matcher = new AntPathMatcher();
    }

    @Test
    void tc01_exactMatch_shouldReturnTrue() {
        assertTrue(matcher.match("machin/truc/file.js", "machin/truc/file.js"));
    }

    @Test
    void tc02_exactMismatch_shouldReturnFalse() {
        assertFalse(matcher.match("/test", "/other"));
    }

    @Test
    void tc03_questionWildcard_shouldMatchOneCharacter() {
        assertTrue(matcher.match("/t?st", "/test"));
    }

    @Test
    void tc04_questionWildcard_shouldFailWhenLengthDoesNotFit() {
        assertFalse(matcher.match("/t?st", "/toast"));
    }

    @Test
    void tc05_singleStar_shouldMatchSinglePathSegment() {
        assertTrue(matcher.match("/test/*", "/test/abc"));
    }

    @Test
    void tc06_singleStar_shouldNotCrossDirectorySeparator() {
        assertFalse(matcher.match("/test/*", "/test/abc/def"));
    }

    @Test
    void tc07_doubleStar_shouldMatchMultipleSegments() {
        assertTrue(matcher.match("/test/**", "/test/abc/def"));
    }

    @Test
    void tc08_doubleStarInMiddle_shouldMatchDeepPath() {
        assertTrue(matcher.match("/**/test.jsp", "/a/b/test.jsp"));
    }

    @Test
    void tc09_uriTemplateRegex_shouldMatchValidFileName() {
        assertTrue(matcher.match("/com/{filename:\\w+}.jsp", "/com/test.jsp"));
    }

    @Test
    void tc10_uriTemplateRegex_shouldRejectWrongExtension() {
        assertFalse(matcher.match("/com/{filename:\\w+}.jsp", "/com/test.html"));
    }

    @Test
    void tc11_absoluteRelativeMismatch_shouldReturnFalse() {
        assertFalse(matcher.match("/test", "test"));
    }

    @Test
    void tc12_emptyPatternAndEmptyPath_shouldReturnTrue() {
        assertTrue(matcher.match("", ""));
    }

    @Test
    void tc13_emptyPatternAndNonEmptyPath_shouldReturnFalse() {
        assertFalse(matcher.match("", "/test"));
    }

    @Test
    void tc14_nullPath_shouldReturnFalse() {
        assertFalse(matcher.match("/test", null));
    }

    @Test
    void tc15_nullPattern_shouldThrowNullPointerException() {
        assertThrows(NullPointerException.class, () -> matcher.match(null, "/test"));
    }

    @Test
    void tc16_caseInsensitiveEnvironment_shouldMatchIgnoringCase() {
        matcher.setCaseSensitive(false);
        assertTrue(matcher.match("/TeSt", "/test"));
    }

    @Test
    void tc17_extractUriTemplateVariables_shouldExtractFilename() {
        Map<String, String> vars =
                matcher.extractUriTemplateVariables("/com/{filename:\\w+}.jsp", "/com/test.jsp");

        assertEquals("test", vars.get("filename"));
    }
}
