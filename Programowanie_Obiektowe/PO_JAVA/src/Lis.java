public class Lis extends Zwierze
{
    public Lis(int x, int y, Swiat swiat)
    {
        super(Stale.LIS_SILA, Stale.LIS_INICJATYWA, x, y, swiat);
    }

    @Override
    public void akcja()
    {
        wiek++;

        int[] XY = swiat.wylosujSasiada(this);
        if (XY != null)
        {
            Organizm cel = swiat.getOrganizmNa(XY[0], XY[1]);
            if (cel == null)        //przesunieto zwierze
            {
                swiat.dodajLog(rysowanie() + " (" + x + ", " + y + ", " + sila + ") przesuwa się na: " + XY[0] + ", " + XY[1]);
                swiat.ustawOrganizmNa(x, y, null);
                x = XY[0];
                y = XY[1];
                swiat.ustawOrganizmNa(x, y, this);
            }
            else                    //zwierze zaatakowalo
            {
                if (cel.getSila() > sila)   //lis wyczul zagrozenie
                {
                    swiat.dodajLog(rysowanie() + " (" + x + ", " + y + ", " + sila + ") wyczuł zagrożenie");
                }
                else
                {
                    cel.kolizja(this);
                }
            }
        }
        else
        {
            swiat.dodajLog(rysowanie() + " (" + x + ", " + y + ", " + sila + ") brak dostepnych ruchow");
        }
    }

    @Override
    public char rysowanie()
    {
        return Stale.LIS;
    }

    @Override
    public String nazwa()
    {
        return Stale.LIS_S;
    }

    @Override
    public Organizm stworzPotomka(int x, int y)
    {
        return new Lis(x, y, swiat);
    }
}