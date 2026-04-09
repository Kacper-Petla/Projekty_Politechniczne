public class Wilk extends Zwierze
{
    public Wilk(int x, int y, Swiat swiat)
    {
        super(Stale.WILK_SILA, Stale.WILK_INICJATYWA, x, y, swiat);
    }

    @Override
    public char rysowanie()
    {
        return Stale.WILK;
    }

    @Override
    public String nazwa()
    {
        return Stale.WILK_S;
    }

    @Override
    public Organizm stworzPotomka(int x, int y)
    {
        return new Wilk(x, y, swiat);
    }
}
