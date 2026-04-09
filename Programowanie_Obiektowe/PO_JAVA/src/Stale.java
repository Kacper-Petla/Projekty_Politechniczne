public class Stale
{
    // Klawisze
    public static final char WYJDZ = 'q';
    public static final char WCZYTAJ = 'w';
    public static final char NOWA_GRA = 'n';

    // Typy organizmów
    public enum TypOrganizmu
    {
        ZWIERZE, ROSLINA
    }

    // Człowiek
    public static final char CZLOWIEK = '@';
    public static final String CZLOWIEK_S = "Czlowiek";
    public static final int CZLOWIEK_SILA = 5;
    public static final int CZLOWIEK_INICJATYWA = 4;

    // Owca
    public static final char OWCA = 'O';
    public static final String OWCA_S = "Owca";
    public static final int OWCA_SILA = 4;
    public static final int OWCA_INICJATYWA = 4;

    // Wilk
    public static final char WILK = 'W';
    public static final String WILK_S = "Wilk";
    public static final int WILK_SILA = 9;
    public static final int WILK_INICJATYWA = 5;

    // Lis
    public static final char LIS = 'L';
    public static final String LIS_S = "Lis";
    public static final int LIS_SILA = 3;
    public static final int LIS_INICJATYWA = 7;

    // Żółw
    public static final char ZOLW = 'Z';
    public static final String ZOLW_S = "Zolw";
    public static final int ZOLW_SILA = 2;
    public static final int ZOLW_INICJATYWA = 1;
    public static final int ZOLW_ODPIERA = 5;

    // Antylopa
    public static final char ANTYLOPA = 'A';
    public static final String ANTYLOPA_S = "Antylopa";
    public static final int ANTYLOPA_SILA = 4;
    public static final int ANTYLOPA_INICJATYWA = 4;

    // Rośliny
    public static final int PRAWDOPODOBIENSTWO_ROZPRZESTRZENIANIA = 10;
    public static final int INICJATYWA_ROSLIN = 0;

    public static final char TRAWA = 't';
    public static final String TRAWA_S = "Trawa";
    public static final int TRAWA_SILA = 0;

    public static final char MLECZ = 'm';
    public static final String MLECZ_S = "Mlecz";
    public static final int MLECZ_SILA = 0;
    public static final int MLECZ_PROBY_ROZPRZESTRZENIENIA = 3;

    public static final char GUARANA = 'g';
    public static final String GUARANA_S = "Guarana";
    public static final int GUARANA_SILA = 0;
    public static final int GUARANA_BOOST_SILA = 3;

    public static final char WILCZA_JAGODA = 'j';
    public static final String WILCZA_JAGODA_S = "Wilcza Jagoda";
    public static final int WILCZA_JAGODA_SILA = 99;

    public static final char BARSZCZ_SOSNOWSKIEGO = 'b';
    public static final String BARSZCZ_SOSNOWSKIEGO_S = "Barszcz Sosnowskiego";
    public static final int BARSZCZ_SOSNOWSKIEGO_SILA = 10;

    //Plansza

    public static final int PLANSZA_X = 500;
    public static final int PLANSZA_Y = 500;

    public static final int FONT_SIZE = 15;

    public static final double PROCENT_PLANSZY = 0.2;
    public static final int LICZBA_ORGANIZMOW = 10;

    public static final char HEX = 'H';
    public static final char KWADRAT = 'K';

    // Inne
    public static final int BOOST_SILY = 10;
    public static final int CZAS_TRWANIA_UMIEJETNOSCI = 5;
    public static final int CZAS_REGENERACJI_UMIEJETNOSCI = 5;

    public static final char CYBER_OWCA = 'C';
}
