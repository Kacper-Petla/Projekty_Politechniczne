public class Trawa extends Roslina
{
    public Trawa(int x, int y, Swiat swiat)
    {
        super(Stale.TRAWA_SILA, x, y, swiat);
    }

    @Override
    public void kolizja(Organizm atakujacy)
    {
        swiat.ustawOrganizmNa(atakujacy.getX(), atakujacy.getY(), null);
        atakujacy.setX(x);
        atakujacy.setY(y);
        swiat.ustawOrganizmNa(x, y, atakujacy);

        if (atakujacy.rysowanie() == Stale.OWCA)
        {
            swiat.dodajLog(rysowanie() + " zjedzony przez " + atakujacy.rysowanie() + " (" + atakujacy.getX() + ", " + atakujacy.getY() + ")");
            swiat.usunOrganizm(this);
        }
        else
        {
            swiat.dodajLog(rysowanie() + " pod " + atakujacy.rysowanie() + " (" + atakujacy.getX() + ", " + atakujacy.getY() + ")");
        }
    }

    @Override
    public char rysowanie()
    {
        return Stale.TRAWA;
    }

    @Override
    public String nazwa()
    {
        return Stale.TRAWA_S;
    }

    @Override
    public Organizm stworzPotomka(int x, int y)
    {
        return new Trawa(x, y, swiat);
    }
}