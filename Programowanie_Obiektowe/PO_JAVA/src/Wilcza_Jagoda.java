public class Wilcza_Jagoda extends Roslina
{
    public Wilcza_Jagoda(int x, int y, Swiat swiat)
    {
        super(Stale.WILCZA_JAGODA_SILA, x, y, swiat);
    }

    @Override
    public void kolizja(Organizm atakujacy)
    {
        swiat.dodajLog(rysowanie() + " (" + x + ", " + y + ") zjedzony przez " + atakujacy.rysowanie() + " (" + atakujacy.getX() + ", " + atakujacy.getY() + ")");
        swiat.dodajLog(atakujacy.rysowanie() + " (" + atakujacy.getX() + ", " + atakujacy.getY() + ") zatruty przez " + rysowanie());

        swiat.ustawOrganizmNa(atakujacy.getX(), atakujacy.getY(), null);
        swiat.ustawOrganizmNa(x, y, null);

        swiat.usunOrganizm(atakujacy);
        swiat.usunOrganizm(this);
    }

    @Override
    public char rysowanie()
    {
        return Stale.WILCZA_JAGODA;
    }

    @Override
    public String nazwa()
    {
        return Stale.WILCZA_JAGODA_S;
    }

    @Override
    public Organizm stworzPotomka(int x, int y)
    {
        return new Wilcza_Jagoda(x, y, swiat);
    }
}