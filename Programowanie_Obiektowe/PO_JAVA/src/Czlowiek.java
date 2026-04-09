public class Czlowiek extends Zwierze
{
    public Czlowiek(int x, int y, Swiat swiat)
    {
        super(Stale.CZLOWIEK_SILA, Stale.CZLOWIEK_INICJATYWA, x, y, swiat);
        this.umiejetnosc = -(Stale.CZAS_TRWANIA_UMIEJETNOSCI + Stale.CZAS_REGENERACJI_UMIEJETNOSCI);
        this.tmp_sila = 0;
    }

    @Override
    public void akcja()
    {
        if (umiejetnosc == wiek)        //umiejetnosc aktywowana
        {
            tmp_sila = sila;
            sila = Stale.BOOST_SILY;
        }
        else if (Stale.CZAS_TRWANIA_UMIEJETNOSCI + umiejetnosc - wiek > 0)      //umiejetnosc aktywna
        {
            sila--;
        }
        else if (Stale.CZAS_TRWANIA_UMIEJETNOSCI + umiejetnosc - wiek == 0)     //umiejentonosc wygasla
        {
            sila = sila - (Stale.BOOST_SILY - Stale.CZAS_TRWANIA_UMIEJETNOSCI + 1) + tmp_sila;
        }

        wiek++;
        przesun(gx, gy);
    }

    @Override
    public void rozmnazanie()
    {
        // Człowiek się nie rozmnaża
    }

    @Override
    public char rysowanie()
    {
        return Stale.CZLOWIEK;
    }

    @Override
    public String nazwa()
    {
        return Stale.CZLOWIEK_S;
    }

    @Override
    public Organizm stworzPotomka(int x, int y)
    {
        return null;
    }
}