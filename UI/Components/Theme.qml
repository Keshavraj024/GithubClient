pragma Singleton
import QtQuick 2.15

QtObject {
    id: root

    property bool isDark: true

    function toggleTheme() {
        isDark = !isDark
    }

    // -----------------------------
    // Palettes
    // -----------------------------
    readonly property QtObject dark: QtObject {
        readonly property color background: "#0f172a"
        readonly property color surface: "#1e293b"
        readonly property color surfaceHover: "#334155"
        readonly property color surfacePressed: "#475569"

        readonly property color cardBackground: "#1e293b"
        readonly property color cardBorder: "#334155"

        readonly property color textPrimary: "#f1f5f9"
        readonly property color textSecondary: "#a8b3cf"
        readonly property color textTertiary: "#64748b"

        readonly property color border: "#334155"
        readonly property color borderLight: "#475569"

        readonly property color glassBackground: Qt.alpha("#1e293b", 0.65)
        readonly property color glassBorder: Qt.alpha("#64748b", 0.2)

        readonly property color disabledText: "#64748b"
        readonly property color disabledBackground: "#1e293b"

        readonly property color shadowLow: Qt.alpha("#000000", 0.25)
        readonly property color shadowMedium: Qt.alpha("#000000", 0.4)
    }

    readonly property QtObject light: QtObject {
        readonly property color background: "#f8fafc"
        readonly property color surface: "#ffffff"
        readonly property color surfaceHover: "#f1f5f9"
        readonly property color surfacePressed: "#e2e8f0"

        readonly property color cardBackground: "#ffffff"
        readonly property color cardBorder: "#e2e8f0"

        readonly property color textPrimary: "#1e293b"
        readonly property color textSecondary: "#64748b"
        readonly property color textTertiary: "#94a3b8"

        readonly property color border: "#cbd5e1"
        readonly property color borderLight: "#e2e8f0"

        readonly property color glassBackground: Qt.alpha("#ffffff", 0.75)
        readonly property color glassBorder: Qt.alpha("#334155", 0.2)

        readonly property color disabledText: "#94a3b8"
        readonly property color disabledBackground: "#e5e7eb"

        readonly property color shadowLow: Qt.alpha("#000000", 0.4)
        readonly property color shadowMedium: Qt.alpha("#000000", 0.12)
    }

    // -----------------------------
    // Active palette (single switch)
    // -----------------------------
    readonly property QtObject palette: isDark ? root.dark : root.light

    // -----------------------------
    // Brand colors (mode-independent)
    // -----------------------------
    readonly property color accent: "#3b82f6"
    readonly property color accentHover: "#2563eb"
    readonly property color accentPressed: "#1d4ed8"

    readonly property color success: "#10b981"
    readonly property color warning: "#f59e0b"
    readonly property color error: "#ef4444"
    readonly property color info: "#06b6d4"

    // -----------------------------
    // Animations
    // -----------------------------
    readonly property int fastAnimation: 150
    readonly property int normalAnimation: 250
    readonly property int slowAnimation: 350

    // -----------------------------
    // Language colors
    // -----------------------------
    function getLanguageColor(language) {
        const colors = {
            "JavaScript": "#f7df1e",
            "TypeScript": "#3178c6",
            "Python": "#3776ab",
            "Java": "#ed8b00",
            "C++": "#00599c",
            "C": "#a8b9cc",
            "C#": "#239120",
            "Go": "#00add8",
            "Rust": isDark ? "#f5f5f5" : "#000000",
            "Swift": "#fa7343",
            "Kotlin": "#7f52ff",
            "Ruby": "#cc342d",
            "PHP": "#777bb4",
            "HTML": "#e34c26",
            "CSS": "#1572b6",
            "Shell": "#89e051",
            "Dart": "#0175c2",
            "Vue": "#4fc08d",
            "React": "#61dafb"
        }

        const langColor = colors[language]
        if (!langColor)
            return palette.textSecondary

        return langColor
    }
}
