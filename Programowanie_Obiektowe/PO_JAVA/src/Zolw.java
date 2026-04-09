public class Zolw extends Zwierze
{
    public Zolw(int x, int y, Swiat swiat)
    {
        super(Stale.ZOLW_SILA, Stale.ZOLW_INICJATYWA, x, y, swiat);
    }

    @Override
    public void akcja()
    {
        wiek++;

        if (new java.util.Random().nextInt(4) == 0)     // 25% szansy na ruch
        {
            int[] XY = swiat.wylosujSasiada(this);
            if (XY != null)
            {
                przesun(XY[0], XY[1]);
            }
            else
            {
                swiat.dodajLog(rysowanie() + " (" + x + ", " + y + ", " + sila + ") brak dostepnych ruchow");
            }
        }
        else
        {
            swiat.dodajLog(rysowanie() + " (" + x + ", " + y + ", " + sila + ") nie rusza się");
        }
    }

    @Override
    public void kolizja(Organizm atakujacy)
    {
        if (this.getClass() == atakujacy.getClass())
        {
            rozmnazanie();
        }
        else
        {
            if (atakujacy.getSila() < Stale.ZOLW_ODPIERA)
            {
                swiat.dodajLog(rysowanie() + " (" + x + ", " + y + ", " + sila + ") odparł atak " + atakujacy.rysowanie() + " (" + atakujacy.getX() + ", " + atakujacy.getY() + ", " + atakujacy.getSila() + ")");
            }
            else
            {
                atak(atakujacy);
            }
        }
    }

    @Override
    public char rysowanie()
    {
        return Stale.ZOLW;
    }

    @Override
    public String nazwa()
    {
        return Stale.ZOLW_S;
    }

    @Override
    public Organizm stworzPotomka(int x, int y)
    {
        return new Zolw(x, y, swiat);
    }
}